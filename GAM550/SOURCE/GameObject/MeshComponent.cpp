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
	m_CastShadows(true),
	m_ReceiveShadows(true),
	m_IsLit(true),
	m_TextureFlags(0)
{
	for (int i = 0; i < TextureType::NUM_TEXTURE_TYPES; ++i) {
		m_Textures[i] = nullptr;
	}
}

MeshComponent::~MeshComponent() {}

void MeshComponent::Deactivate() 
{
	mp_Parent = nullptr;
	for (int i = 0; i < TextureType::NUM_TEXTURE_TYPES; ++i) {
		m_Textures[i] = nullptr;
	}
}

void MeshComponent::Update(float dt) {}

void MeshComponent::Serialize(const json& j)
{
	m_TextureFlags = 0;
	mp_Scene = INFECT_RESOURCES.GetScene(ParseString(j, "scene"));
	if (ValueExists(j, "diffuse")) {
		m_Textures[TextureType::DiffuseTexture] = INFECT_RESOURCES.GetTexture(ParseString(j, "diffuse"));
		m_TextureFlags |= DIFFUSE_TEXTURED;
	}
	if (ValueExists(j, "normal")) {
		m_TextureFlags |= NORMAL_MAPPED;
		m_Textures[TextureType::NormalMap] = INFECT_RESOURCES.GetTexture(ParseString(j, "normal"));
	}
	if (ValueExists(j, "specular")) {
		m_TextureFlags |= SPECULAR_MAPPED;
		m_Textures[TextureType::SpecularMap] = INFECT_RESOURCES.GetTexture(ParseString(j, "specular"));
	}
	if (ValueExists(j, "isLit"))
		m_IsLit = ParseBool(j, "isLit");
	m_IsDirty = true;
}

void MeshComponent::Override(const json& j) 
{
	if (ValueExists(j, "scene"))
		mp_Scene = INFECT_RESOURCES.GetScene(ParseString(j, "scene"));
	if (ValueExists(j, "diffuse")) {
		m_Textures[TextureType::DiffuseTexture] = INFECT_RESOURCES.GetTexture(ParseString(j, "diffuse"));
		m_TextureFlags |= DIFFUSE_TEXTURED;
	}
	if (ValueExists(j, "normal")) {
		m_TextureFlags |= NORMAL_MAPPED;
		m_Textures[TextureType::NormalMap] = INFECT_RESOURCES.GetTexture(ParseString(j, "normal"));
	}
	if (ValueExists(j, "specular")) {
		m_TextureFlags |= SPECULAR_MAPPED;
		m_Textures[TextureType::SpecularMap] = INFECT_RESOURCES.GetTexture(ParseString(j, "specular"));
	}
	if (ValueExists(j, "isLit"))
		m_IsLit = ParseBool(j, "isLit");
	m_IsDirty = true;
}

void MeshComponent::SetScene(const std::string & sceneName)
{
	mp_Scene = INFECT_RESOURCES.GetScene(sceneName);
	m_IsDirty = true;
}

void MeshComponent::SetDiffuseTexture(const std::string & textureName)
{
	m_Textures[TextureType::DiffuseTexture] = INFECT_RESOURCES.GetTexture(textureName);
	m_IsDirty = true;
}

void MeshComponent::SetNormalTexture(const std::string & textureName)
{
	m_Textures[TextureType::NormalMap] = INFECT_RESOURCES.GetTexture(textureName);
	m_IsDirty = true;
}

void MeshComponent::SetSpecularTexture(const std::string & textureName)
{
	m_Textures[TextureType::SpecularMap] = INFECT_RESOURCES.GetTexture(textureName);
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
