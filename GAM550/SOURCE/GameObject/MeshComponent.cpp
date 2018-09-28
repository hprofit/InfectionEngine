/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MeshComponent::MeshComponent(InfectGUID guid) :
	RenderableComponent(guid),
	mp_Scene(nullptr),
	mp_Diffuse(nullptr),
	m_CastShadows(true),
	m_ReceiveShadows(true),
	m_IsLit(true)
{
}

MeshComponent::~MeshComponent() {}

void MeshComponent::Deactivate() 
{
	mp_Parent = nullptr; 
}

void MeshComponent::Update(float dt) {}

void MeshComponent::Serialize(const json& j)
{
	mp_Scene = INFECT_RESOURCES.GetScene(ParseString(j, "scene"));
	if(ValueExists(j, "texture"))
		mp_Diffuse = INFECT_RESOURCES.GetTexture(ParseString(j, "texture"));
	if (ValueExists(j, "isLit"))
		m_IsLit = ParseBool(j, "isLit");
	m_IsDirty = true;
}

void MeshComponent::Override(const json& j) 
{

}

void MeshComponent::SetScene(const std::string & sceneName)
{
	mp_Scene = INFECT_RESOURCES.GetScene(sceneName);
	m_IsDirty = true;
}

void MeshComponent::SetDiffuseTexture(const std::string & textureName)
{
	mp_Diffuse = INFECT_RESOURCES.GetTexture(textureName);
	m_IsDirty = true;
}

void MeshComponent::SetNormalTexture(const std::string & textureName)
{
	mp_NormalMap = INFECT_RESOURCES.GetTexture(textureName);
	m_IsDirty = true;
}

void MeshComponent::SetSpecularTexture(const std::string & textureName)
{
	mp_SpecMap = INFECT_RESOURCES.GetTexture(textureName);
	m_IsDirty = true;
}

void MeshComponent::SetCastShadows(bool castShadows)
{
	m_CastShadows = castShadows;
	m_IsDirty = true;
}

void MeshComponent::SetReceiveShadows(bool receiveShadows)
{
	m_ReceiveShadows = receiveShadows;
	m_IsDirty = true;
}

void MeshComponent::SetIsLit(bool isLit)
{
	m_IsLit = isLit;
	m_IsDirty = true;
}
