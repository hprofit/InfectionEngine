/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

PointLightComponent::PointLightComponent(InfectGUID guid) :
	LightBaseComponent(guid, false),
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
	SetScene(ParseString(j, "volumeShape"));
}

void PointLightComponent::Override(const json & j)
{
	LightBaseComponent::Serialize(j);
	if (ValueExists(j, "volumeShape")) 
		SetScene(ParseString(j, "volumeShape"));
}

void PointLightComponent::SetScene(const std::string & sceneName)
{
	mp_Scene = INFECT_RESOURCES.GetScene(sceneName);
	m_IsDirty = true;
}