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
	mp_Diffuse(nullptr)
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

}

void MeshComponent::Override() 
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
