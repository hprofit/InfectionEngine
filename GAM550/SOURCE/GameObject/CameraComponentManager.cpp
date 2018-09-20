/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

CameraComponentManager::CameraComponentManager() {
	m_Components = (std::list<CameraComponent*>*)INFECT_MEMORY.GetComponentPool(CameraComponent::Type);
}

void CameraComponentManager::_CalcViewMatrix(CC comp)
{
	TransformComponent* tComp = comp->Parent()->GetComponent<TransformComponent>();
	Matrix4x4 rotM = tComp->GetRotationMatrix();
	comp->m_viewMatrix = rotM * Matrix4x4::Translate(-1 * tComp->WorldPosition());
}

void CameraComponentManager::Update(float dt)
{
	for each (CC cComp in *m_Components) {
		if (!cComp->IsActive()) continue; // will be removed or modified later 

		cComp->m_UpdatedLastFrame = false;
		if (cComp->IsDirty() || cComp->Parent()->GetComponent<TransformComponent>()->UpdatedLastFrame()) {
			_CalcViewMatrix(cComp);
			switch (cComp->m_projectionType) {
				case ProjectionType::PT_Orthographic: {
					float width = (float)INFECT_RENDERER.ScreenWidth();
					float height = (float)INFECT_RENDERER.ScreenHeight();
					cComp->m_cameraMatrix = Matrix4x4::Orthographic(width, height, cComp->m_Near, cComp->m_Far);
					break;
				}
				case ProjectionType::PT_Perspective:
				default: {
					cComp->m_cameraMatrix = Matrix4x4::Perspective(cComp->m_fov, INFECT_RENDERER.AspectRatio(), cComp->m_Near, cComp->m_Far);
					break;
				}
			}
			cComp->m_finalMatrix = cComp->m_cameraMatrix * cComp->m_viewMatrix;

			cComp->m_IsDirty = false;
			cComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void CameraComponentManager::HandleEvent(Event * pEvent)
{
	for each (CC tComp in *m_Components) {
		tComp->HandleEvent(pEvent);
	}
}

void CameraComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<CameraComponent>(CameraComponent::Type);
}