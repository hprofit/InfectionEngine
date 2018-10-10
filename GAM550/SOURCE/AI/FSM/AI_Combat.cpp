/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Combat::AI_Combat()
: AI_State(AI_State_Combat) {

}

AI_Combat::~AI_Combat(){

}

void AI_Combat::OnEnter(){
	// Init
	m_pBT->Init(*m_pBT->GetRootNode(), nullptr);
}

void AI_Combat::OnUpdate(float dt) {
	m_pBT->Tick();

}

void AI_Combat::OnExit(){
	m_pBT->ClearScheduler();

}

void AI_Combat::HandleEvent(Event* pEvent) {

}

//void AI_Combat::Serialize(const json& j) {
//}