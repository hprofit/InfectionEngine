/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef BT_SEQUENCE_H
#define BT_SEQUENCE_H

/**************************************************
* Behavior Tree - Control Node
* LOGIC: execute children in order until one fails
* RETURN:	BH_SUCCESS if all children success
*			BH_FAILURE if any child fails
****************************************************/
class Sequence: public Composite{
protected:
	BehaviorTree* m_pBT;
	vector<Behavior*>::iterator m_currentNode;
public:
	Sequence(BehaviorTree& tree, int size);
	~Sequence();

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
	virtual void OnChildComplete(BH_Status);
};

#endif