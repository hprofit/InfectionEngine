/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef BT_SIMPLEPARALLEL_H
#define BT_SIMPLEPARALLEL_H

/**************************************************
* Behavior Tree - Control Node
* LOGIC: execute all children in a single tick
* RETURN:	N/A (undecided)
****************************************************/
class SimpleParallel: public Composite{
protected:
	BehaviorTree* m_pBT;
	vector<Behavior*>::iterator m_currentNode;
public:
	SimpleParallel() {};
	~SimpleParallel() {};

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
	virtual void OnChildComplete(BH_Status);
};

#endif