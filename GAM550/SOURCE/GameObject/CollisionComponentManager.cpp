/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

CollisionComponentManager::CollisionComponentManager() {
  m_Components = (std::list<CollisionComponent*>*)INFECT_MEMORY.GetComponentPool(CollisionComponent::Type);
}

void CollisionComponentManager::Update(float dt)
{
  for each (RBC plComp in *m_Components) {
    if (!plComp->IsActive()) break; 
    plComp->m_UpdatedLastFrame = false;
    if (plComp->IsDirty()) {

      plComp->m_IsDirty = false;
      plComp->m_UpdatedLastFrame = true;
    }
  }
}

// TODO: Super naive attempt at this
void CollisionComponentManager::HandleEvent(Event * pEvent)
{
  for each (RBC plComp in *m_Components) {
    if (!plComp->IsActive()) break;
    plComp->HandleEvent(pEvent);
  }
}

void CollisionComponentManager::Init() {
  INFECT_MEMORY.ComponentPoolInit<CollisionComponent>(CollisionComponent::Type, CollisionComponent::CACHESIZE);
}