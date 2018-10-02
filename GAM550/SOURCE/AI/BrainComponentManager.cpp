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
	for each (BC cComp in *m_Components) {
		if (!cComp->IsActive()) break;
		// Change of state
		//if (cComp->m_pCurrentState != cComp->m_pPreviousState) {
		//	if (m_AIStates[m_currentState]) {
		//		m_AIStates[m_previousState]->OnExit();
		//		m_AIStates[m_currentState]->OnEnter();
		//		m_previousState = m_currentState;
		//	}
		//	else {
		//		m_currentState = m_previousState;
		//	}
		//}

		//// Update with currentState
		//if (m_AIStates[m_currentState])
		//	m_AIStates[m_currentState]->OnUpdate(dt);

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