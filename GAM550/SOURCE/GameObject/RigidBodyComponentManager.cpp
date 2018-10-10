
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
 
#include <Stdafx.h>

RigidBodyComponentManager::RigidBodyComponentManager() {
  m_Components = (std::list<RigidBodyComponent*>*)INFECT_MEMORY.GetComponentPool(RigidBodyComponent::Type);
}


void RigidBodyComponentManager::Update(float dt)
{
  Vector3D rotation;
  for each (RBC rgbComp in *m_Components) { 
	  if (!rgbComp->IsActive()) break;
	  switch (rgbComp->cur_type)
	  {
	  case RigidBodyComponent::RigidBodyType::BoxRigidBody:
      if (rgbComp->GetBoxPointer()->m_hit)
        rgbComp->SetFractureHit();

		  rgbComp->Parent()->GetComponent<TransformComponent>()->SetPosition(rgbComp->GetBoxPointer()->body->getPosition());
      rotation = RotationMatrixToEulerAngles(rgbComp->GetBoxPointer()->body->getTransform());
      rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngles(rotation.x, rotation.y, rotation.z);
		  break;

	  case RigidBodyComponent::RigidBodyType::SphereRigidBody:
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetPosition(rgbComp->GetSpherePointer()->body->getPosition());
        rotation = RotationMatrixToEulerAngles(rgbComp->GetSpherePointer()->body->getTransform());
        rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngles(rotation.x, rotation.y, rotation.z);
			  
		 break;
	  }
	
  }
}

// TODO: Super naive attempt at this
void RigidBodyComponentManager::HandleEvent(Event * pEvent)
{
  for each (RBC plComp in *m_Components) {
    if (!plComp->IsActive()) break;
    plComp->HandleEvent(pEvent);
  }
}

void RigidBodyComponentManager::Init() {
  INFECT_MEMORY.ComponentPoolInit<RigidBodyComponent>(RigidBodyComponent::Type, RigidBodyComponent::CACHESIZE);
}