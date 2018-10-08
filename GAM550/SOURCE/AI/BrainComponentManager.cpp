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

		/*
			FOR JEREMY: 
				Since the BrainComponent seems to need to update every frame (and others
				like it may need to do the same), I'm removing the dirty check for now.
				If you find you still need it for any other reason, it's still there for you 
				to use.

				-Holden
		*/

		// Change of state
		if (bComp->m_currentState != bComp->m_previousState) {
			if (bComp->m_states[bComp->m_currentState]) {
				bComp->m_states[bComp->m_previousState]->OnExit();
				bComp->m_states[bComp->m_currentState]->OnEnter();
				bComp->m_previousState = bComp->m_currentState;
			}
			else {
				bComp->m_currentState = bComp->m_previousState;
			}
		}

		// Update with currentState
		if (bComp->m_states[bComp->m_currentState])
			bComp->m_states[bComp->m_currentState]->OnUpdate(dt);
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