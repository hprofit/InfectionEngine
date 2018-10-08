/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

TransformComponentManager::TransformComponentManager() {
	m_Components = (std::list<TransformComponent*>*)INFECT_MEMORY.GetComponentPool(TransformComponent::Type);
}

void TransformComponentManager::_UpdateLookAt(TC tComp)
{
	tComp->m_rotation =
		Matrix4x4::Rotate(tComp->m_angleZ, ZAXIS) *
		Matrix4x4::Rotate(tComp->m_angleY, YAXIS) *
		Matrix4x4::Rotate(tComp->m_angleX, XAXIS);
	tComp->m_lookAt = Vector3D::Normalize(tComp->m_rotation * (Vector3D(0, 0, -1.0f, 0)));
	tComp->m_lookAt.w = 0;
}

void TransformComponentManager::_UpdateRightUp(TC tComp)
{
	Vector3D lCrossR = Vector3D::Cross(tComp->m_lookAt, YAXIS);
	float lenLCrossR = lCrossR.Length();
	tComp->m_right = lenLCrossR != 0.0f ? lCrossR * (1.0f / lenLCrossR) : ZAXIS;

	tComp->m_up = Vector3D::Cross(tComp->m_lookAt*-1, tComp->m_right);
}

void TransformComponentManager::_UpdateTransform(TC tComp)
{
	Matrix4x4 trans = Matrix4x4::Translate(tComp->m_pivotOffset + tComp->m_position);
	Matrix4x4 scale = Matrix4x4::Scale(tComp->m_scaleX, tComp->m_scaleY, tComp->m_scaleZ);

	tComp->m_transform = trans * tComp->m_rotation * scale;
	tComp->m_worldPosition = tComp->m_position;

	// If the GO has a parent GO, we need to concatenate the transform matricies and add the parent's World Pos to this 
	// transform's World Pos
	if (tComp->Parent()->HasParent()) {
		TC parentTransform = tComp->Parent()->Parent()->GetComponent<TransformComponent>();
		tComp->m_worldPosition += parentTransform->m_worldPosition;
		tComp->m_transform = parentTransform->GetTransform() * tComp->m_transform;
	}
}

void TransformComponentManager::Update(float dt)
{
	for each (TransformComponent * tComp in *m_Components) {
		if (!tComp->IsActive()) break;
		tComp->m_UpdatedLastFrame = false;
		if (tComp->IsDirty()) {
			tComp->m_prevPosition = tComp->m_position;

			_UpdateLookAt(tComp);
			_UpdateRightUp(tComp);
			_UpdateTransform(tComp);
			
			tComp->m_IsDirty = false;
			tComp->m_UpdatedLastFrame = true;
		}
	}
}

// TODO: Super naive attempt at this
void TransformComponentManager::HandleEvent(Event * pEvent)
{
	for each (TransformComponent * tComp in *m_Components) {
		if (!tComp->IsActive()) break;
		tComp->HandleEvent(pEvent);
	}
}

void TransformComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<TransformComponent>(TransformComponent::Type, TransformComponent::CACHESIZE);
}