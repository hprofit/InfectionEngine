 /* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef BT_ATTACK_H
#define BT_ATTACK_H

class Attack: public Action{
protected:
	float m_sinceOnInit;
	float m_attackDelay;
	bool m_attackExecuted;
public:
	Attack(BehaviorTree*);
	~Attack() {};

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
	void Serialize(const json& j);
};

#endif