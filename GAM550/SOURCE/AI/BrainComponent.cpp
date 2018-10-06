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

void BrainComponent::LateInitialize() {
}

void BrainComponent::Serialize(const json& j){
	int stateSize = j["ai_states"].size();
	
	for (unsigned i = 0; i < stateSize; i++) {
		std::string stateName = ParseString(j["ai_states"][i], "state");
		AI_State* newState = INFECT_MEMORY.GetNewState(stateName);
		newState->pAgent = this;
		newState->SetBehaviorTree( INFECT_RESOURCES.GetBehaviorTree(this, ParseString(j["ai_states"][i], "behaviorTree")));

		m_states[STATE_NAMES[stateName]] = newState;
	}

	m_detectionRadius = ParseFloat(j, "detectionRadius");
	m_IdleMovementRadius = 30;
	// hard coded for ai system test (engine demo)
	//m_currentState = m_previousState = AI_State_Idle;
	//m_states[0]= new AI_Idle() ;
	//m_states[0]->Serialize(j);
	//m_IsDirty = true;
}

void BrainComponent::Override(const json& j){
	//m_ActiveAreaRadius = 20;
	m_InitPos = Vector3D(Parent()->GetComponent<TransformComponent>()->WorldPosition());
}

void BrainComponent::HandleEvent(Event * p_event){

}

bool BrainComponent::isDestinationReached(const Vector3D& targetPos){
	return Vector3D::SquareDistance(Parent()->GetComponent<TransformComponent>()->WorldPosition(), targetPos) < 200.0f;
	//return (targetPos == Parent()->GetComponent<TransformComponent>()->WorldPosition());
}

void BrainComponent::GetRandomPos(Vector3D& targetPos){
	float offset1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2.0f - 1.0f;
	float offset2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2.0f - 1.0f;
	float offset3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX)*2.0f - 1.0f;
	targetPos.Set(	m_InitPos.x + m_IdleMovementRadius * offset1 ,
					m_InitPos.y + m_IdleMovementRadius * offset2 ,
					m_InitPos.z + m_IdleMovementRadius * offset3	);
	//targetPos.Print();
}

void BrainComponent::MoveTo(const Vector3D& targetPos) { 
	// TODO: do it through physics component
	TransformComponent* pTrans = Parent()->GetComponent<TransformComponent>();
	Vector3D moveVec = (targetPos - pTrans->WorldPosition());
	moveVec.Normalize();
	float spd = 20.0f;
	pTrans->Move(moveVec*INFECT_FRAMERATE.GetFrameTime()*spd);  
}