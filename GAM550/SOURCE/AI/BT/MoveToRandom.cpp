 /* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MoveToRandom::MoveToRandom(BehaviorTree* tree):m_destination(Vector3D(0,0,0)){
	SetStatus(BH_INVALID);
	//pBT = tree;
}

void MoveToRandom::OnInitialize() {
	//pAgent->GetRandomPos(m_destination);
	AI_Commands::getRandomPos(	pAgent->GetInitPos(), 
								pAgent->GetMovementRadius(),
								m_destination);
}

BH_Status MoveToRandom::OnUpdate() {
	if (AI_Commands::isDestinationReached(pAgent, m_destination)) {
		return BH_SUCCESS;
	}
	else {
		AI_Commands::moveTo(pAgent, m_destination);
		return BH_RUNNING;
	}
}

void MoveToRandom::OnTerminate(BH_Status) {
	SetStatus(BH_INVALID);
}


void MoveToRandom::Serialize(const json& j) {

}