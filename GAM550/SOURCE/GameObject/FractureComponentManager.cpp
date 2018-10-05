/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

FractureComponentManager::FractureComponentManager() {
  m_Components = (std::list<FractureComponent*>*)INFECT_MEMORY.GetComponentPool(FractureComponent::Type);
}

void FractureComponentManager::Update(float dt)
{
  for each (FC plComp in *m_Components) {
    if (plComp)
      plComp->Update(dt);
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