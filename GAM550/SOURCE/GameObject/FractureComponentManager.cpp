/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void FractureComponentManager::_DivideBlock(FC fc, RigidBodyComponent::Box * blocks)
{
	// Find out if block one or two in the contact structure,
	Vector3D normal = fc->mp_Contact->contactNormal;
	physics::RigidBody *body = fc->mp_Contact->body[0];
	if (body != fc->mp_Target->body)
	{
		normal.Negate();
		body = fc->mp_Contact->body[1];
	}

	// Work out where on the body (in body coordinates) the contact is
	// and its direction.
	Vector3D point = body->getPointInLocalSpace(fc->mp_Contact->contactPoint);
	normal = body->getDirectionInLocalSpace(normal);

	// Work out the centre of the split:
	point = point - normal * (point * normal);

	// Take a copy of the half size, so we can create the new blocks.
	Vector3D size = fc->mp_Target->halfSize;

	// Take a copy also of the body's other data.
	physics::RigidBody tempBody;
	tempBody.setPosition(body->getPosition());
	tempBody.setOrientation(body->getOrientation());
	tempBody.setVelocity(body->getVelocity());
	tempBody.setRotation(body->getRotation());
	tempBody.setLinearDamping(body->getLinearDamping());
	tempBody.setAngularDamping(body->getAngularDamping());
	tempBody.setInverseInertiaTensor(body->getInverseInertiaTensor());
	tempBody.calculateDerivedData();


	// Work out the inverse density of the old block
	real invDensity =
		fc->mp_Target->halfSize.Length() * 8 * body->getInverseMass();

	// Remove the old block
	INFECT_PHYSICS.m_BoxPool.remove(fc->mp_Target);


	// Now split the block into eight.
	for (unsigned i = 0; i < 8; i++)
	{
		// Find the minimum and maximum extents of the new block
		// in old-block coordinates
		Vector3D min, max;
		if ((i & 1) == 0) {
			min.x = -size.x;
			max.x = point.x;
		}
		else {
			min.x = point.x;
			max.x = size.x;
		}
		if ((i & 2) == 0) {
			min.y = -size.y;
			max.y = point.y;
		}
		else {
			min.y = point.y;
			max.y = size.y;
		}
		if ((i & 4) == 0) {
			min.z = -size.z;
			max.z = point.z;
		}
		else {
			min.z = point.z;
			max.z = size.z;
		}

		// Get the origin and half size of the block, in old-body
		// local coordinates.
		Vector3D halfSize = (max - min) * 0.5f;
		Vector3D newPos = halfSize + min;

		// Convert the origin to world coordinates.
		newPos = tempBody.getPointInWorldSpace(newPos);

		// Work out the direction to the impact.
		Vector3D direction = newPos - fc->mp_Contact->contactPoint;
		direction.normalise();

		// Set the body's properties (we assume the block has a body
		// already that we're going to overwrite).
		blocks[i].body->setPosition(newPos);
		blocks[i].body->setVelocity(tempBody.getVelocity() + direction * 10.0f);
		blocks[i].body->setOrientation(tempBody.getOrientation());
		blocks[i].body->setRotation(tempBody.getRotation());
		blocks[i].body->setLinearDamping(tempBody.getLinearDamping());
		blocks[i].body->setAngularDamping(tempBody.getAngularDamping());
		blocks[i].body->setAwake(true);
		blocks[i].body->setAcceleration(Vector3D::GRAVITY);
		blocks[i].body->clearAccumulators();
		blocks[i].body->calculateDerivedData();
		blocks[i].offset = Matrix4x4(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 0.f);;
		blocks[i].halfSize = halfSize;

		// Finally calculate the mass and inertia tensor of the new block
		blocks[i].calculateMassProperties(invDensity);

		//add to box pool
		INFECT_PHYSICS.m_BoxPool.push_back(&blocks[i]);
	}
}

FractureComponentManager::FractureComponentManager() {
  m_Components = (std::list<FractureComponent*>*)INFECT_MEMORY.GetComponentPool(FractureComponent::Type);
}

void FractureComponentManager::Update(float dt)
{
	for each (FC plComp in *m_Components) {
		if (!plComp->IsActive()) break;
		if (plComp->IsDirty()) {

			if (plComp->m_Hit)
			{
				RigidBodyComponent::Box* blocks = new RigidBodyComponent::Box[8];
				_DivideBlock(plComp, blocks);
				plComp->m_Hit = false;
			}

			plComp->m_IsDirty = false;
			plComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void FractureComponentManager::HandleEvent(Event * pEvent)
{
  for each (FC plComp in *m_Components) {
    if (!plComp->IsActive()) break;
    plComp->HandleEvent(pEvent);
  }
}

void FractureComponentManager::Init() {
  INFECT_MEMORY.ComponentPoolInit<FractureComponent>(FractureComponent::Type, FractureComponent::CACHESIZE);
}