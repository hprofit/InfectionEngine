/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

static const std::string COMPONENTS = "COMPONENTS";

// Macro trick to make Component types enums from the file ComponentTypesNames.h
#define REGISTER_COMPONENT_TYPE(x) x,
typedef enum
{
#include "ComponentTypesNames.h"
	NUM_COMPONENTS
} ComponentType;
#undef REGISTER_COMPONENT_TYPE

#define REGISTER_COMPONENT_TYPE(x) #x,
static const char* ComponentTypeText[] =
{
#include "ComponentTypesNames.h"
	"Invalid"
};
#undef REGISTER_COMPONENT_TYPE

static std::unordered_map<std::string, ComponentType> COMPONENT_NAMES = {
	{ "Transform", C_Transform },
	{ "Mesh", C_Mesh },
	{ "Camera", C_Camera },
	{ "PointLight", C_PointLight },
	{ "Collision", C_Collision },
	{ "RigidBody", C_RigidBody },
	{ "Fracture", C_Fracture },
	{ "CollisionTrigger", C_CollisionTrigger },
	{ "Brain", C_Brain },
	{ "ParticleEmitter", C_ParticleEmitter },
	{ "Controller", C_Controller }
};

#endif // !COMPONENT_TYPES_H
