/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

MeshComponent::MeshComponent() :
	RenderableComponent(ComponentType::C_Mesh),
	mp_Scene(nullptr)
{
}

MeshComponent::~MeshComponent() {}

void MeshComponent::Deactivate() 
{
	pGO = nullptr; 
}

void MeshComponent::Update(float dt) {}

void MeshComponent::Serialize()
{

}

void MeshComponent::Override() 
{

}
