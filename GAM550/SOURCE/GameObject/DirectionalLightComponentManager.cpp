/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

DirectionalLightComponentManager::DirectionalLightComponentManager() {
	m_Components = (std::list<DirectionalLightComponent*>*)INFECT_MEMORY.GetComponentPool(DirectionalLightComponent::Type);
}

void DirectionalLightComponentManager::_CalcViewPerspMatrix(DLC comp)
{
	TransformComponent* tComp = comp->Parent()->GetComponent<TransformComponent>();
	Matrix4x4 viewMat = tComp->GetRotationMatrix() * Matrix4x4::Translate(-1 * tComp->WorldPosition());
	comp->m_ViewPerspMat = Matrix4x4::Perspective(comp->m_FoV, comp->m_AspectRatio, 0.1f, comp->m_Far) * viewMat;
	comp->m_ShadowMat = comp->m_ViewPerspMat;
	comp->m_ShadowMat.Transpose();
}

void DirectionalLightComponentManager::Update(float dt)
{
	for each (DLC dlComp in *m_Components) {
		if (!dlComp->IsActive()) break; 
		dlComp->m_UpdatedLastFrame = false;
		if (dlComp->IsDirty() || dlComp->Parent()->GetComponent<TransformComponent>()->UpdatedLastFrame())
		{
			_CalcViewPerspMatrix(dlComp);

			dlComp->m_IsDirty = false;
			dlComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void DirectionalLightComponentManager::HandleEvent(Event * pEvent)
{
	for each (DLC dlComp in *m_Components) {
		if (!dlComp->IsActive()) break;
		dlComp->HandleEvent(pEvent);
	}
}

void DirectionalLightComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<DirectionalLightComponent>(DirectionalLightComponent::Type, DirectionalLightComponent::CACHESIZE);
}