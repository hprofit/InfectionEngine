/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef AI_FACTORY_H
#define AI_FACTORY_H

// forward declaration
class AI_State;
class BehaviorTree;

class AI_Factory{
public:
	AI_Factory();
	~AI_Factory(){};
	AI_Factory(const AI_Factory &) = delete;
	void operator=(const AI_Factory &) = delete;

	AI_State* CreateState(std::string state);
	//BehaviorTree* CreateBT(std::string btName);
private:
	typedef AI_State* CreationFunction();
	std::unordered_map <AIStateType, CreationFunction*> m_CreationFunctions;
};

#endif