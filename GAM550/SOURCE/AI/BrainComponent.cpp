/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

BrainComponent::BrainComponent(InfectGUID guid):
	Component(guid){

}

void BrainComponent::Deactivate(){

}

void BrainComponent::Update(float dt) {
	// Change of state
	if (m_currentState != m_previousState) {
		if (m_states[m_currentState]) {
			m_states[m_previousState]->OnExit();
			m_states[m_currentState]->OnEnter();
			m_previousState = m_currentState;
		}
		else {
			m_currentState = m_previousState;
		}
	}
	// Update with currentState
	if (m_states[m_currentState])
		m_states[m_currentState]->OnUpdate(dt);
}

void BrainComponent::Serialize(const json& j){

}

void BrainComponent::Override(const json& j){

}

void BrainComponent::HandleEvent(Event * p_event){

}