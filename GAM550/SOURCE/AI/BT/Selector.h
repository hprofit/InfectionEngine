/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef BT_SELECTOR_H
#define BT_SELECTOR_H

/**************************************************
* Behavior Tree - Control Node
* LOGIC: execute children in order until one success
* RETURN:	BH_SUCCESS if any child success
*			BH_FAILURE if all children fail
****************************************************/
class Selector: public Composite{
protected:
	BehaviorTree* pBT;
	vector<Behavior*>::iterator m_currentNode;
public:
	Selector() {};
	~Selector() {};

	virtual void OnInitialize();
	virtual BH_Status OnUpdate();
	virtual void OnTerminate(BH_Status);
	virtual void OnChildComplete(BH_Status);
};

#endif