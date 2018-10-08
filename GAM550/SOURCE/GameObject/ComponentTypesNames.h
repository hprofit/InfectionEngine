/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

/*
All component types are listed here. These are component names that will be 
turned into entries within the ComponentType enum
*/ 

REGISTER_COMPONENT_TYPE(C_Transform)

// Rendering Components
REGISTER_COMPONENT_TYPE(C_Mesh)
REGISTER_COMPONENT_TYPE(C_ParticleEmitter)
REGISTER_COMPONENT_TYPE(C_Camera)

// Light Components
REGISTER_COMPONENT_TYPE(C_PointLight)

//physics
REGISTER_COMPONENT_TYPE(C_Collision)
REGISTER_COMPONENT_TYPE(C_RigidBody)
REGISTER_COMPONENT_TYPE(C_CollisionTrigger)
REGISTER_COMPONENT_TYPE(C_Fracture)
// AI
REGISTER_COMPONENT_TYPE(C_Brain)
REGISTER_COMPONENT_TYPE(C_Controller)