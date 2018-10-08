/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_COMBAT_H
#define AI_COMBAT_H

class AI_Combat : public AI_State {
public:
	AI_Combat();
	~AI_Combat();
	static AI_State* CreateInstance() { return new AI_Combat(); }

	virtual void OnEnter();
	virtual void OnUpdate(float);
	virtual void OnExit(); 
	virtual void HandleEvent(Event* pEvent);
	//virtual void Serialize(const json& j);

private:
	int m_attackCounter;
	int m_attackMaxLimit;
	int m_attackMinLimit;
	float idleTime;
	float idleDuration;
};

#endif