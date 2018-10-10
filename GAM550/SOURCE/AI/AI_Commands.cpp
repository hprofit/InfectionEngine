/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#include <Stdafx.h>

bool AI_Commands::isDestinationReached(BrainComponent* pAgent, const Vector3D& targetPos) {
	return Vector3D::SquareDistance(pAgent->Parent()->GetComponent<TransformComponent>()->WorldPosition(), targetPos) < 200.0f;
}


bool AI_Commands::isPlayerDetected(BrainComponent* pAgent, const float radius){
	TransformComponent* pPlayerTrans = INFECT_GOM.GetCamera()->GetComponent<TransformComponent>();
	TransformComponent* pAgentTrans = pAgent->Parent()->GetComponent<TransformComponent>();
	return (Vector3D::SquareDistance(pAgentTrans->WorldPosition(), pPlayerTrans->WorldPosition()) < radius*radius);
}

void AI_Commands::getRandomPos(const Vector3D& initialPos, const float moveRadius, Vector3D& targetPos) {
	targetPos.Set(initialPos.x + moveRadius * Rand_Zero_One()*2.0f - 1.0f ,
		initialPos.y + moveRadius * Rand_Zero_One()*2.0f - 1.0f,
		initialPos.z + moveRadius * Rand_Zero_One()*2.0f - 1.0f);
}


void AI_Commands::moveTo(BrainComponent* pAgent, const Vector3D& targetPos) {
	// TODO: do it through physics component
	TransformComponent* pTrans = pAgent->Parent()->GetComponent<TransformComponent>();
	Vector3D moveVec = (targetPos - pTrans->WorldPosition());
	moveVec.Normalize();
	float spd = 20.0f;
	pTrans->Move(moveVec*INFECT_FRAMERATE.GetFrameTime()*spd);
}

void AI_Commands::changeState(BrainComponent* pAgent, AIStateType statetype) {
	pAgent->SetState(statetype);
}

void AI_Commands::changeColor(BrainComponent* pAgent, float r, float g, float b, float a) {
	MeshComponent* pMesh = pAgent->Parent()->GetComponent<MeshComponent>();
	if (pMesh) {
		// change color here
	}
}