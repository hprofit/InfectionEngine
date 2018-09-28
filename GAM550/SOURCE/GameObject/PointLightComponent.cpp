/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

PointLightComponent::PointLightComponent(InfectGUID guid) :
	LightBaseComponent(guid),
	mp_Scene(INFECT_RESOURCES.GetScene(SPHERE_PRIMITIVE))
{
}

void PointLightComponent::Deactivate()
{
	mp_Parent = nullptr;
}

void PointLightComponent::Serialize(const json & j)
{
	LightBaseComponent::Serialize(j);
	mp_Scene = INFECT_RESOURCES.GetScene(ParseString(j, "volumeShape"));
	m_IsDirty = true;
}

void PointLightComponent::SetScene(const std::string & sceneName)
{
	mp_Scene = INFECT_RESOURCES.GetScene(sceneName);
	m_IsDirty = true;
}