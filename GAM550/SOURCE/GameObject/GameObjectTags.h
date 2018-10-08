/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAME_OBJECT_TAGS_H
#define GAME_OBJECT_TAGS_H

// Macro trick to make Tags enums from the file TagNames.h
#define REGISTER_TAG_NAME(x) x,
typedef enum
{
#include "TagNames.h"

	T_None
} GameObjectTag;
#undef REGISTER_TAG_NAME

#define REGISTER_TAG_NAME(x) #x,
static const char* TagNameText[] =
{
#include "TagNames.h"
	"Invalid"
};
#undef REGISTER_TAG_NAME

static std::unordered_map<std::string, GameObjectTag> TAG_NAMES = {
	{ "Player", T_Player },
	{ "Camera", T_Camera },
	{ "Light", T_Light },
    { "Bullet", T_Bullet},
	{ "NONE", T_None },
	{ "None", T_None }

};

#endif