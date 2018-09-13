/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

CameraComponent::CameraComponent() :
	//Component(ComponentType::C_Camera),
	Component(),
	m_fov(105.f), m_aspectRatio(1.f), m_Near(0.1f), m_Far(1000.0f),
	m_viewMatrix(Matrix4x4()), m_cameraMatrix(Matrix4x4()), m_projectionType(ProjectionType::PT_Perspective), m_Depth(0)
{
}

CameraComponent::~CameraComponent(){}

void CameraComponent::Deactivate() {
	mp_Parent = nullptr;
	m_pTransform = nullptr; 
}

Matrix4x4 CameraComponent::_MatrixFromCameraVectors(const Vector3D & right, const Vector3D & up, const Vector3D & forward)
{
	return Matrix4x4(
		right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		-forward.x, -forward.y, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

void CameraComponent::_CalcViewMatrix()
{
	Matrix4x4 rotationM = _MatrixFromCameraVectors(m_pTransform->Right(), m_pTransform->Up(), m_pTransform->Forward());
	m_viewMatrix = rotationM * Matrix4x4::Translate(-1 * m_pTransform->GetPosition());
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

void CameraComponent::LateInitialize()
{
	if (!m_pTransform) {
		assert(mp_Parent && "No Game Object found. CameraComponent failed to operate.");
		m_pTransform = mp_Parent->GetComponent<TransformComponent>();
		assert(m_pTransform && "No TransformComponent found. CameraComponent requires a TransformComponent.");

		INFECT_GOM.RegisterCamera(mp_Parent);
	}
}

void CameraComponent::Update(float dt)
{
}

void CameraComponent::LateUpdate(float dt)
{
	_CalcViewMatrix();
	switch (m_projectionType) {
		case ProjectionType::PT_Orthographic: {
			float width = (float)INFECT_RENDERER.ScreenWidth();
			float height = (float)INFECT_RENDERER.ScreenHeight();
			m_cameraMatrix = Matrix4x4::Orthographic(width, height, m_Near, m_Far);
			break;
		}
		case ProjectionType::PT_Perspective:
		default: {
			m_cameraMatrix = Matrix4x4::Perspective(m_fov, INFECT_RENDERER.AspectRatio(), m_Near, m_Far);
			break;
		}
	}
	m_finalMatrix = m_cameraMatrix * m_viewMatrix;
}

Vector3D CameraComponent::TransformPointToScreenSpace(const Vector3D& worldCoordinates) {
	Matrix4x4 viewPerspectiveMatrix = GetCameraMatrix() * GetViewMatrix();

	// Transform point to clipping coordinates
	Vector3D result = viewPerspectiveMatrix * worldCoordinates;
	//result.x = ((result.x / result.w) + 1.f) / 2.0f * m_screenWidth*m_zoomLevels[m_currentZoomIndex];
	//result.y = (1 - (result.y / result.w)) / 2.0f * m_screenHeight * m_zoomLevels[m_currentZoomIndex];
	//result.z = 0;
	//result.w = 1;

	return result;
}