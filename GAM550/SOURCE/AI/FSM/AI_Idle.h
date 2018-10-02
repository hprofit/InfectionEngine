/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_IDLE_H
#define AI_IDLE_H

//#include "AI_State.h"

class AI_Idle : public AI_State {
public:
	AI_Idle();
	~AI_Idle();
	static AI_State* CreateInstance() { return new AI_Idle(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit();
	virtual void HandleEvent(Event* pEvent);
	virtual void Serialize(const json& j);

private:
	float idleDuration; // in seconds
	float idledSoFar;
};

#endif