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

void AI_Commands::getRandomPos(const Vector3D& initialPos, const float moveRadius, Vector3D& targetPos) {
	float offset1 = Rand_Zero_One()*2.0f - 1.0f;
	float offset2 = Rand_Zero_One()*2.0f - 1.0f;
	float offset3 = Rand_Zero_One()*2.0f - 1.0f;
	targetPos.Set(initialPos.x + moveRadius * offset1,
		initialPos.y + moveRadius * offset2,
		initialPos.z + moveRadius * offset3);
}


void AI_Commands::moveTo(BrainComponent* pAgent, const Vector3D& targetPos) {
	// TODO: do it through physics component
	TransformComponent* pTrans = pAgent->Parent()->GetComponent<TransformComponent>();
	Vector3D moveVec = (targetPos - pTrans->WorldPosition());
	moveVec.Normalize();
	float spd = 20.0f;
	pTrans->Move(moveVec*INFECT_FRAMERATE.GetFrameTime()*spd);
}