/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

AIStateFactory::AIStateFactory(){
	//m_CreationFunctions[StateText[NPC_State_Idle]] = AI_Idle::CreateInstance;
}

AI_State* AIStateFactory::CreateState(std::string state){
	CreationFunction *Create = m_CreationFunctions[state];
	if (Create){
		return Create();
	}
	return nullptr;
}