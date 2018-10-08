/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef AI_STATETYPE_H
#define AI_STATETYPE_H

// Macro trick to make state enums from the file AI_StateTypeNames.h
#define REGISTER_STATE(x) x,
typedef enum
{
#include "AI_StateTypeNames.h"
	NUM_AI_STATES
} AIStateType;
#undef REGISTER_STATE

#define REGISTER_STATE(x) #x,
static const char* StateText[] =
{
#include "AI_StateTypeNames.h"
	"Invalid"
};
#undef REGISTER_STATE

static std::unordered_map<std::string, AIStateType> STATE_NAMES = {
	{ "ai_state_idle", AI_State_Idle },
	{ "ai_state_combat", AI_State_Combat }

};

#endif
