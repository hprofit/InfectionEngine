/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef BT_CONDITIONALLOOP_H
#define BT_CONDITIONALLOOP_H

// funtion pointer for condition
// maybe define a functor class?
typedef std::function<bool ()>Condition;

class ConditionalLoop: public Decorator{
protected:
	BehaviorTree* pBT;
	Condition m_CondiFunc;
public:
	ConditionalLoop(BehaviorTree& tree, Condition& cond);
	~ConditionalLoop();

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
	virtual void OnChildComplete(BH_Status);

};

#endif