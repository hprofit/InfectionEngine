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
	m_Color(CompressedColor(0,0,0,0)),
	m_Specular(Color(0.9f, 0.9f, 0.9f, 50.0f)),
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

#define SCENE_STRING "scene"
#define DIFFUSE_TEXTURE_STRING "diffuse"
#define NORMAL_TEXTURE_STRING "normal"
#define SPECULAR_TEXTURE_STRING "specular"
#define OVERRIDE_COLOR_STRING "color"
#define SPECULAR_VALUES_STRING "specularValues"

void MeshComponent::Serialize(const json& j)
{
	m_TextureFlags = 0;
	SetScene(ParseString(j, SCENE_STRING));
	if (ValueExists(j, DIFFUSE_TEXTURE_STRING))
		SetDiffuseTexture(ParseString(j, DIFFUSE_TEXTURE_STRING));
	if (ValueExists(j, NORMAL_TEXTURE_STRING))
		SetNormalTexture(ParseString(j, NORMAL_TEXTURE_STRING));
	if (ValueExists(j, SPECULAR_TEXTURE_STRING))
		SetSpecularTexture(ParseString(j, SPECULAR_TEXTURE_STRING));
	//if (ValueExists(j, "isLit"))
	//	m_IsLit = ParseBool(j, "isLit");
	if (ValueExists(j, OVERRIDE_COLOR_STRING))
		SetOverrideColor(ParseCompColor(j, OVERRIDE_COLOR_STRING));
	if (ValueExists(j, SPECULAR_VALUES_STRING))
		SetSpecularValues(ParseColor(j, SPECULAR_VALUES_STRING));
}

void MeshComponent::Override(const json& j) 
{
	if (ValueExists(j, SCENE_STRING))
		mp_Scene = INFECT_RESOURCES.GetScene(ParseString(j, SCENE_STRING));
	if (ValueExists(j, DIFFUSE_TEXTURE_STRING))
		SetDiffuseTexture(ParseString(j, DIFFUSE_TEXTURE_STRING));
	if (ValueExists(j, NORMAL_TEXTURE_STRING))
		SetNormalTexture(ParseString(j, NORMAL_TEXTURE_STRING));
	if (ValueExists(j, SPECULAR_TEXTURE_STRING))
		SetSpecularTexture(ParseString(j, SPECULAR_TEXTURE_STRING));
	//if (ValueExists(j, "isLit"))
	//	m_IsLit = ParseBool(j, "isLit");
	if (ValueExists(j, OVERRIDE_COLOR_STRING))
		SetOverrideColor(ParseCompColor(j, OVERRIDE_COLOR_STRING));
	if (ValueExists(j, SPECULAR_VALUES_STRING))
		SetSpecularValues(ParseColor(j, SPECULAR_VALUES_STRING));
}

#undef SCENE_STRING
#undef DIFFUSE_TEXTURE_STRING
#undef NORMAL_TEXTURE_STRING
#undef SPECULAR_TEXTURE_STRING
#undef OVERRIDE_COLOR_STRING
#undef SPECULAR_VALUES_STRING

void MeshComponent::SetScene(const std::string & sceneName)
{
	mp_Scene = INFECT_RESOURCES.GetScene(sceneName);
	m_IsDirty = true;
}

void MeshComponent::SetDiffuseTexture(const std::string & textureName)
{
	m_Textures[TextureType::DiffuseTexture] = INFECT_RESOURCES.GetTexture(textureName);
	m_TextureFlags |= DIFFUSE_TEXTURED;
	m_IsDirty = true;
}

void MeshComponent::SetNormalTexture(const std::string & textureName)
{
	m_Textures[TextureType::NormalMap] = INFECT_RESOURCES.GetTexture(textureName);
	m_TextureFlags |= NORMAL_MAPPED;
	m_IsDirty = true;
}

void MeshComponent::SetSpecularTexture(const std::string & textureName)
{
	m_Textures[TextureType::SpecularMap] = INFECT_RESOURCES.GetTexture(textureName);
	m_TextureFlags |= SPECULAR_MAPPED;
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

void MeshComponent::SetOverrideColor(const CompressedColor & color)
{
	m_TextureFlags |= COLOR_OVERRIDE;
	m_Color = color;
	m_IsDirty = true;
}

void MeshComponent::SetSpecularValues(const Color & specular)
{
	m_Specular = specular;
	m_IsDirty = true;
}
