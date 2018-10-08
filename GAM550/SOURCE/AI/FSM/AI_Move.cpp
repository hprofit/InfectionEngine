/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Move::AI_Move()
	: AI_State(AI_State_Move) {

}

AI_Move::~AI_Move() {
}

void AI_Move::OnEnter() {

}

void AI_Move::OnUpdate(float dt) {

}

void AI_Move::OnExit(){
	
}


void AI_Move::HandleEvent(Event* pEvent) {

}

void AI_Move::Serialize(const json& j) {
}