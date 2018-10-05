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
  for each (RBC plComp in *m_Components) {
    if (plComp)
      plComp->Update(dt);
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