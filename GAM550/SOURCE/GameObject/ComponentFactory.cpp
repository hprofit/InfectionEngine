/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

InfectGUID ComponentFactory::_GetGUID() const
{
	return INFECT_GUID.GetGUID();
}

ComponentFactory::ComponentFactory() {
	// Component Constructor Function
	m_creationFunctions[TransformComponent::Type] = TransformComponent::CreateInstance;
	m_creationFunctions[MeshComponent::Type] = MeshComponent::CreateInstance;
	m_creationFunctions[CameraComponent::Type] = CameraComponent::CreateInstance;
	m_creationFunctions[PointLightComponent::Type] = PointLightComponent::CreateInstance;
	m_creationFunctions[RigidBodyComponent::Type] = RigidBodyComponent::CreateInstance;
	m_creationFunctions[BrainComponent::Type] = BrainComponent::CreateInstance;
}

void ComponentFactory::RegisterComponent(ComponentType cType) {

}