 /* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef BT_MOVETORANDOM_H
#define BT_MOVETORANDOM_H

class MoveToRandom: public Action{
protected:
	Vector3D m_destination;
public:
	MoveToRandom(BehaviorTree*);
	~MoveToRandom() {};

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
	void Serialize(const json& j);
};

#endif