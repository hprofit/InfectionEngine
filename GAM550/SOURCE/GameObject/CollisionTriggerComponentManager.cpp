/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

CollisionTriggerComponentManager::CollisionTriggerComponentManager() {
  m_Components = (std::list<CollisionTriggerComponent*>*)INFECT_MEMORY.GetComponentPool(CollisionTriggerComponent::Type);
}

void CollisionTriggerComponentManager::Update(float dt)
{
  for each (RBC plComp in *m_Components) {
    if (!plComp->IsActive()) break; // will be removed or modified later 
    plComp->m_UpdatedLastFrame = false;
    if (plComp->IsDirty()) {

      plComp->m_IsDirty = false;
      plComp->m_UpdatedLastFrame = true;
    }
  }
}

// TODO: Super naive attempt at this
void CollisionTriggerComponentManager::HandleEvent(Event * pEvent)
{
  for each (RBC plComp in *m_Components) {
    if (!plComp->IsActive()) break;
    plComp->HandleEvent(pEvent);
  }
}

void CollisionTriggerComponentManager::Init() {
  INFECT_MEMORY.ComponentPoolInit<CollisionTriggerComponent>(CollisionTriggerComponent::Type, CollisionTriggerComponent::CACHESIZE);
}