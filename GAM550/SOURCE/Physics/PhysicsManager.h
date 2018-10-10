
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
#pragma once

#ifndef PHYSICSMANAGER_MANAGER_H
#define PHYSICSMANAGER_MANAGER_H
class Subscriber;
class CollisionComponent;
class PhysicsManager : public Subscriber
{
public:
	PhysicsManager();
	~PhysicsManager();

	 void Init();
	 void Update(float deltaTime);
     void ErrorCheck(FMOD_RESULT result);




	std::list<RigidBodyComponent::Box *> m_BoxPool;
	std::list<RigidBodyComponent::Sphere *> m_SpherePool;
	std::list<RigidBodyComponent::Plane *> m_PlanePool;

	void CollisionTrigger();

	void AddToBoxPool(RigidBodyComponent::Box *box);

	void AddToSpherePool(RigidBodyComponent::Sphere *sphere);

	void AddToPlanePool(RigidBodyComponent::Plane *plane);


	void SetCollisionData(real friction_ = (real)0.9, real restitution_ = (real)0.1, real tolerance_ = (real)0.1);

	physics::Contact& GetFractureContact() {return cData.contactArray[fracture_contact];}

private:
	real m_CollisionData[3];
	// collision info
	const static unsigned maxContacts = 256;
	physics::Contact contacts[maxContacts];
	physics::CollisionData cData;
	physics::ContactResolver resolver = physics::ContactResolver(maxContacts * 8);

	// fracture
	unsigned fracture_contact;

};


#endif