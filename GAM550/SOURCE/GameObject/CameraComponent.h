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
	Matrix4x4 m_viewMatrix, m_cameraMatrix;			// View matrix and perspective/orthographic matrix
	bool m_primary;										// True if this camera is the game's primary camera
	ProjectionType m_projectionType;					// True if this camera is Perspective, false if Orthographic

	Matrix4x4 _MatrixFromCameraVectors(const Vector3D& right, const Vector3D& up, const Vector3D& forward);
	void _CalcViewMatrix();
public:
	CameraComponent();
	~CameraComponent();
	static Component* CreateInstance() { return new CameraComponent(); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize();
	virtual void HandleEvent(Event* pEvent);

	Vector3D TransformPointToScreenSpace(const Vector3D& worldCoordinates);

	float GetFOV() const;
	float GetAspect() const;
	Matrix4x4 GetViewMatrix() const { return m_viewMatrix; };
	Matrix4x4 GetCameraMatrix() const { return m_cameraMatrix; };
};

#endif

