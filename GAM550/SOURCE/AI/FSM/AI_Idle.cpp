/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Idle::AI_Idle()
: AI_State(AI_State_Idle) {
	idleDuration = RandomFloat(1, 3); // get 1 - 3 sec
	idledSoFar = 0.0f;
}

AI_Idle::~AI_Idle(){
}

void AI_Idle::OnEnter(){
	// reset idle data
	idleDuration = RandomFloat(1, 3); 
	idledSoFar = 0.0f;
}

void AI_Idle::OnUpdate(float dt){
	if (idledSoFar < idleDuration) {
		idledSoFar += dt;
	}
}

void AI_Idle::OnExit(){
	
}


void AI_Idle::HandleEvent(Event* pEvent) {

}

void AI_Idle::Serialize(const json& j) {

}