 /* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#ifndef AI_COMMANDS_H
#define AI_COMMANDS_H

namespace AI_Commands{
	// check if agent has reached the targetPos specified
	bool isDestinationReached(BrainComponent* pAgent, const Vector3D& targetPos);
	// get random position within the zone (moveRadius from the initialPos) and set to targetPos
	void getRandomPos(const Vector3D& initialPos, const float moveRadius, Vector3D& targetPos );
	// move agent toward the targetPos
	void moveTo (BrainComponent* pAgent, const Vector3D& targetPos);
}

#endif