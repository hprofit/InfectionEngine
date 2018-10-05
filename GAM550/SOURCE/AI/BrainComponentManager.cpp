/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

BrainComponentManager::BrainComponentManager() {
	m_Components = (std::list<BrainComponent*>*)INFECT_MEMORY.GetComponentPool(BrainComponent::Type);
}

void BrainComponentManager::Update(float dt) {
	for each (BC bComp in *m_Components) {
		if (!bComp->IsActive()) break;
		if (bComp->IsDirty())
			bComp->Update(dt);
	}
}

// TODO: Super naive attempt at this
void BrainComponentManager::HandleEvent(Event * pEvent) {
	for each (BC tComp in *m_Components) {
		if (!tComp->IsActive()) break;
		tComp->HandleEvent(pEvent);
	}
}

void BrainComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<BrainComponent>(BrainComponent::Type, BrainComponent::CACHESIZE);
}