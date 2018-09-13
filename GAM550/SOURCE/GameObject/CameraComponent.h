/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

enum class ProjectionType {
	PT_Perspective,
	PT_Orthographic
};

class CameraComponent : public Component
{
private:
	TransformComponent* m_pTransform;

	float m_fov, m_aspectRatio;
	float m_Near, m_Far;
	Matrix4x4 m_viewMatrix,				// View, perspective / orthographic, and view*(persp or ortho) matrix
		m_cameraMatrix, 
		m_finalMatrix;			
	int m_Depth;
	ProjectionType m_projectionType;	// True if this camera is Perspective, false if Orthographic

	Matrix4x4 _MatrixFromCameraVectors(const Vector3D& right, const Vector3D& up, const Vector3D& forward);
	void _CalcViewMatrix();
public:
	static const ComponentType Type = ComponentType::C_Camera;
	virtual ComponentType GetType() const { return Type; }

	CameraComponent(InfectGUID guid);
	~CameraComponent();
	static Component* CreateInstance(InfectGUID guid) { return new CameraComponent(guid); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);

	static bool LeftDepthGreaterThanRight(GameObject* left, GameObject* right);

	Vector3D TransformPointToScreenSpace(const Vector3D& worldCoordinates);

	inline float CameraComponent::GetFOV() const { return m_fov; }
	inline float CameraComponent::GetAspect() const	{ return m_aspectRatio;	}
	inline Matrix4x4 GetViewMatrix() const { return m_viewMatrix; };
	inline Matrix4x4 GetCameraMatrix() const { return m_cameraMatrix; }
	inline Matrix4x4 GetFinalCamMatrix() const { return m_finalMatrix; }
	inline int Depth() const { return m_Depth; }
	inline ProjectionType ProjectionType() const { return m_projectionType; }
};

#endif

