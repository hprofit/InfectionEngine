/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AI_Factory::AI_Factory(){
	m_CreationFunctions[AI_State_Idle] = AI_Idle::CreateInstance;
	m_CreationFunctions[AI_State_Combat] = AI_Idle::CreateInstance;
}

AI_State* AI_Factory::CreateState(std::string state){
	CreationFunction *Create = m_CreationFunctions[STATE_NAMES[state]];
	if (Create){
		return Create();
	}
	return nullptr;
}


//BehaviorTree* AI_Factory::CreateBT(std::string btName) {
//	return INFECT_RESOURCES.GetBehaviorTree(btName);
//}