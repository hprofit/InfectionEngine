/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

class CameraComponentManager;
enum class ProjectionType {
	PT_Perspective,
	PT_Orthographic
};

class CameraComponent : public Component
{
protected:
	friend CameraComponentManager;

	float m_fov, m_aspectRatio;
	float m_Near, m_Far;
	Matrix4x4 m_viewMatrix,				// View, perspective / orthographic, and view*(persp or ortho) matrix
		m_cameraMatrix, 
		m_finalMatrix;			
	int m_Depth;
	ProjectionType m_projectionType;	// True if this camera is Perspective, false if Orthographic

	//Matrix4x4 _MatrixFromCameraVectors(const Vector3D& right, const Vector3D& up, const Vector3D& forward);
	//void _CalcViewMatrix();
public:
	static const ComponentType Type = ComponentType::C_Camera;
	static const unsigned CACHESIZE = 5;
	virtual ComponentType GetType() const { return Type; }

	CameraComponent(InfectGUID guid);
	~CameraComponent();
	static Component* CreateInstance(InfectGUID guid) { return new CameraComponent(guid); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j) {}
	virtual void HandleEvent(Event* pEvent);

	static bool LeftDepthGreaterThanRight(GameObject* left, GameObject* right);

	inline Matrix4x4 GetViewMatrix() const { return m_viewMatrix; };
	inline Matrix4x4 GetCameraMatrix() const { return m_cameraMatrix; }
	inline Matrix4x4 GetFinalCamMatrix() const { return m_finalMatrix; }

	inline float GetFOV() const { return m_fov; }
	void SetFOV(float fov);

	inline float GetAspect() const { return m_aspectRatio; }

	inline float GetNear() const { return m_Near; }
	void SetNear(float _near);

	inline float GetFar() const { return m_Far; }
	void SetFar(float _far);

	inline int Depth() const { return m_Depth; }
	void SetDepth(int depth);

	inline ProjectionType GetProjectionType() const { return m_projectionType; }
	void SetProjectionType(ProjectionType pType);
};

#endif

