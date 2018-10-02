/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

class TransformComponentManager;

class TransformComponent : public Component {
protected:
	friend TransformComponentManager;

	Vector3D m_position;					// Current position this frame
	Vector3D m_prevPosition;				// Position last frame

	Vector3D m_pivotOffset;					// Translation away from Parent object's position
	Vector3D m_worldPosition;				// Current position this frame + any offsets from Parent objects
	float m_scaleX, m_scaleY, m_scaleZ;		// Scale in the X, Y, and Z directions

	Matrix4x4 m_transform;					// Model Matrix (Pivot * T * R * S)

	float m_angleX, m_angleY, m_angleZ;		// Angle of rotation along the X, Y, and Z axis
	Matrix4x4 m_rotation;					// Rotation matrix (Zrot * Yrot * Xrot)
	Vector3D m_lookAt;						// Direction the object is facing, default is -ZAXIS, this is calculated from the rotation matrix
	Vector3D m_right;						// Right vector of this transform
	Vector3D m_up;							// Up vector of this transform

public:
	static const unsigned CACHESIZE = 300;
	static const ComponentType Type = ComponentType::C_Transform;
	virtual ComponentType GetType() const { return Type; }

	TransformComponent(InfectGUID guid);
	~TransformComponent() {};

	static Component* CreateInstance(InfectGUID guid) { return new TransformComponent(guid);  }
	virtual void Deactivate();
	virtual void Update(float dt) {};
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	virtual void HandleEvent(Event * p_event);

	void SetPosition(const Vector3D& pos);
	inline Vector3D LocalPosition() const { return m_position; }
	inline Vector3D WorldPosition() const { return m_worldPosition; }
	void Move(const Vector3D& amount);
	void MoveAlongLookAt(Vector3D& amount);
	Vector3D GetMovement() const { return m_worldPosition - m_prevPosition; }

	void SetAngles(float angleX, float angleY, float angleZ);
	void SetAngleX(float angle);
	void SetAngleY(float angle);
	void SetAngleZ(float angle);
	inline float GetAngleX() const { return m_angleX; }
	inline float GetAngleY() const { return m_angleY; }
	inline float GetAngleZ() const { return m_angleZ; }

	void SetPivotOffset(Vector3D offset);
	inline Vector3D GetPivotOffset() const { return m_pivotOffset; }

	void RotateX(float amount);
	void RotateY(float amount);
	void RotateZ(float amount);
	inline Vector3D GetRotVector() const { return Vector3D(m_angleX, m_angleY, m_angleZ); }

	inline Vector3D Forward() const { return m_lookAt; }
	inline Vector3D Right() const { return m_right; }
	inline Vector3D Up() const { return m_up; }

	inline Vector3D LookAt() const { return m_lookAt; }

	inline float GetScaleX() const { return m_scaleX; }
	void SetScaleX(float scaleX);
	void ScaleXby(float amount);

	inline float GetScaleY() const { return m_scaleY; }
	void SetScaleY(float scaleY);
	void ScaleYby(float amount);

	inline float GetScaleZ() const { return m_scaleZ; }
	void SetScaleZ(float scaleZ);
	void ScaleZby(float amount);

	void SetScaleUniform(float amount);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetScale(const Vector3D& scale);
	void ScaleUniform(float amount);
	inline Vector3D GetScaleVector() const { return Vector3D(m_scaleX, m_scaleY, m_scaleZ, 0); }

	inline Matrix4x4 GetTransform() const { return m_transform; }
	Matrix4x4 GetRotationMatrix() const;
};

#endif

