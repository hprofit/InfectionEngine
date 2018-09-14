/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MeshComponent::MeshComponent(InfectGUID guid) :
	RenderableComponent(guid),
	mp_Scene(nullptr)
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
