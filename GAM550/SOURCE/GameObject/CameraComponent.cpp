/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

CameraComponent::CameraComponent(InfectGUID guid) :
	Component(guid),
	m_fov(90.f), 
	m_aspectRatio(1.f), 
	m_Near(0.1f),
	m_Far(1000.0f),
	m_viewMatrix(Matrix4x4()), 
	m_cameraMatrix(Matrix4x4()), 
	m_projectionType(ProjectionType::PT_Perspective), 
	m_Depth(0)
{
}

CameraComponent::~CameraComponent(){}

void CameraComponent::Deactivate() {
	mp_Parent = nullptr;
}

void CameraComponent::Serialize(const json& j)
{

}

void CameraComponent::HandleEvent(Event * pEvent)
{
	switch (pEvent->Type()) {
	default:
		break;
	}
}

bool CameraComponent::LeftDepthGreaterThanRight(GameObject * left, GameObject * right)
{
	CameraComponent* cLeft = left->GetComponent<CameraComponent>();
	CameraComponent* cRight = right->GetComponent<CameraComponent>();

	return cLeft->m_Depth > cRight->m_Depth;
}

void CameraComponent::SetFOV(float fov)
{
	m_fov = fov;
	m_IsDirty = true;
}

void CameraComponent::SetNear(float _near)
{
	m_Near = _near;
	m_IsDirty = true;
}

void CameraComponent::SetFar(float _far)
{
	m_Far= _far;
	m_IsDirty = true;
}

void CameraComponent::SetDepth(int depth)
{
	m_Depth = depth;
	m_IsDirty = true;
}

void CameraComponent::SetProjectionType(ProjectionType pType)
{
	m_projectionType = pType;
	m_IsDirty = true;
}

void CameraComponent::LateInitialize()
{
	INFECT_GOM.RegisterCamera(mp_Parent);
}