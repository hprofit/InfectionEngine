/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

PointLightComponentManager::PointLightComponentManager() {
	m_Components = (std::list<PointLightComponent*>*)INFECT_MEMORY.GetComponentPool(PointLightComponent::Type);
}

void PointLightComponentManager::Update(float dt)
{
	for each (PLC plComp in *m_Components) {
		if (!plComp->IsActive()) continue; // will be removed or modified later 
		plComp->m_UpdatedLastFrame = false;
		if (plComp->IsDirty()) {

			plComp->m_IsDirty = false;
			plComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void PointLightComponentManager::HandleEvent(Event * pEvent)
{
	for each (PLC plComp in *m_Components) {
		plComp->HandleEvent(pEvent);
	}
}

void PointLightComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<PointLightComponent>(PointLightComponent::Type);
}