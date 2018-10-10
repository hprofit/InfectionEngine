
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>


PhysicsManager::PhysicsManager()
{
}
PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Init()
{
	SetCollisionData();
	cData.contactArray = contacts;
}

void PhysicsManager::ErrorCheck(FMOD_RESULT result)
{

}

void PhysicsManager::Update(float deltaTime)
{
	for (std::list<RigidBodyComponent::Box *>::iterator it = m_BoxPool.begin(); it != m_BoxPool.end(); ++it)
	{
		(*it)->body->integrate(deltaTime);
		(*it)->calculateInternals();
	}

	for (std::list<RigidBodyComponent::Sphere *>::iterator it = m_SpherePool.begin(); it != m_SpherePool.end(); ++it)
	{
		(*it)->body->integrate(deltaTime);
		(*it)->calculateInternals();
	}
	// Set up the collision data structure
	cData.reset(maxContacts);
	cData.friction = m_CollisionData[0];
	cData.restitution = m_CollisionData[1];
	cData.tolerance = m_CollisionData[2];

	// loading collision
	CollisionTrigger();

	// solve collision
	resolver.resolveContacts(
		cData.contactArray,
		cData.contactCount,
		INFECT_FRAMERATE.GetFrameTime()
	);
}

//-----------
//has fracture component
      /*if (((*it)->m_CanBeBroken == true && (*it_other)->m_Breaker == true) ||
        ((*it)->m_Breaker == true && (*it_other)->m_CanBeBroken == true))
      {
        if (physics::CollisionDetector::boxAndSphere(*(*it), *(*it_other), &cData))
        {
          if ((*it)->m_CanBeBroken)
          {
            (*it)->m_hit = true;
            (*it)->m_CanBeBroken = false;
            fracture_contact = cData.contactCount - 1;
          }
        }
      }*/
void PhysicsManager::CollisionTrigger()
{

  //physics::CollisionDetector::boxAndBox(*m_BoxPool[0], *m_BoxPool[1], &cData);
  
	for (std::list<RigidBodyComponent::Box *>::iterator it = m_BoxPool.begin(); it != m_BoxPool.end(); ++it)
	{
		// check box and Sphere
		for (std::list<RigidBodyComponent::Sphere *>::iterator it_other = m_SpherePool.begin(); it_other != m_SpherePool.end(); ++it_other)
		{
			// Check ground box-sphere collisions
			if (!cData.hasMoreContacts()) return;
			//physics::CollisionDetector::boxAndSphere(*(*it), *(*it_other), &cData);
      if (((*it)->m_CanBeBroken == true && (*it_other)->m_Breaker == true))
      {
        if (physics::CollisionDetector::boxAndSphere(*(*it), *(*it_other), &cData))
        {
          if ((*it)->m_CanBeBroken)
          {
            (*it)->m_hit = true;
            (*it)->m_CanBeBroken = false;
            fracture_contact = cData.contactCount - 1;
          }
        }
      }
      else
        physics::CollisionDetector::boxAndSphere(*(*it), *(*it_other), &cData);


		}

		// check box and box
		//if (!cData.hasMoreContacts()) return;
		for (std::list<RigidBodyComponent::Box *>::iterator it_other = m_BoxPool.begin(); it_other != m_BoxPool.end(); ++it_other)
		{
			if (*it != *it_other)
			{
				if (!cData.hasMoreContacts()) return;
				  physics::CollisionDetector::boxAndBox(*(*it), *(*it_other), &cData);
			}
		}

		// check box and plane 
		for (std::list<RigidBodyComponent::Plane *>::iterator it_other = m_PlanePool.begin(); it_other != m_PlanePool.end(); ++it_other)
		{
			if (!cData.hasMoreContacts()) return;
			physics::CollisionDetector::boxAndHalfSpace(*(*it), *(*it_other), &cData);
		}
	}

	for (std::list<RigidBodyComponent::Sphere *>::iterator it = m_SpherePool.begin(); it != m_SpherePool.end(); ++it)
	{
		for (std::list<RigidBodyComponent::Sphere *>::iterator it_other = m_SpherePool.begin(); it_other != m_SpherePool.end(); ++it_other)
		{
			if (!cData.hasMoreContacts()) return;
			// Check ground sphere-sphere collisions
			if (*it != *it_other)
				physics::CollisionDetector::sphereAndSphere(*(*it), *(*it_other), &cData);
		}

		// check sphere and plane 
		for (std::list<RigidBodyComponent::Plane *>::iterator it_other = m_PlanePool.begin(); it_other != m_PlanePool.end(); ++it_other)
		{
			if (!cData.hasMoreContacts()) return;
			physics::CollisionDetector::sphereAndHalfSpace(*(*it), *(*it_other), &cData);
		}
	}

}

void PhysicsManager::AddToBoxPool(RigidBodyComponent::Box *box)
{
	m_BoxPool.push_back(box);
}

void PhysicsManager::AddToSpherePool(RigidBodyComponent::Sphere *sphere)
{
	m_SpherePool.push_back(sphere);
}

void PhysicsManager::AddToPlanePool(RigidBodyComponent::Plane *plane)
{
	m_PlanePool.push_back(plane);
}

void PhysicsManager::SetCollisionData(real friction_, real restitution_, real tolerance_)
{
	m_CollisionData[0] = friction_;
	m_CollisionData[1] = restitution_;
	m_CollisionData[2] = tolerance_;
}