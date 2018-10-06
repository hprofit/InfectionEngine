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

	// Sets the GameObject's locatl position relative to the parent object
	void SetPosition(const Vector3D& pos);
	// Returns the GameObject's position relative to the parent object
	// If there is no parent object, this will be the same as WorldPosition
	inline Vector3D LocalPosition() const { return m_position; }
	// Returns the GameObject's position relative to the origin
	inline Vector3D WorldPosition() const { return m_worldPosition; }
	// Translates the GameObject in the direction given
	void Move(const Vector3D& amount);
	// Translates the GameObject along it's own current lookat
	void MoveAlongLookAt(Vector3D& amount);
	// Returns the amount this GameObject has moved since the previous frame
	Vector3D GetMovement() const { return m_worldPosition - m_prevPosition; }

	// Sets the rotation around the x, y, and z axis, angles are in degrees
	void SetAngles(float angleX, float angleY, float angleZ);
	// Sets the rotation around the x axis, angle is in degrees
	void SetAngleX(float angle);
	// Sets the rotation around the y axis, angle is in degrees
	void SetAngleY(float angle);
	// Sets the rotation around the z axis, angle is in degrees
	void SetAngleZ(float angle);
	// Returns the rotation around the x axis in degrees
	inline float GetAngleX() const { return m_angleX; }
	// Returns the rotation around the y axis in degrees
	inline float GetAngleY() const { return m_angleY; }
	// Returns the rotation around the z axis in degrees
	inline float GetAngleZ() const { return m_angleZ; }

	// Sets the GameObject's pivot offset
	void SetPivotOffset(Vector3D offset);
	// Returns the GameObject's pivot offset
	inline Vector3D GetPivotOffset() const { return m_pivotOffset; }

	// Rotates the object about the x axis by the given amount in degrees
	void RotateX(float amount);
	// Rotates the object about the y axis by the given amount in degrees
	void RotateY(float amount);
	// Rotates the object about the z axis by the given amount in degrees
	void RotateZ(float amount);
	// Returns a Vector3D with the x rotation, y rotation, and z rotation as the x, y, and z components all in degrees
	inline Vector3D GetRotVector() const { return Vector3D(m_angleX, m_angleY, m_angleZ); }

	// Returns the GameObject's current lookat direction
	inline Vector3D LookAt() const { return m_lookAt; }
	// Returns the GameObject's current lookat direction
	inline Vector3D Forward() const { return m_lookAt; }
	// Returns the GameObject's current right direction
	inline Vector3D Right() const { return m_right; }
	// Returns the GameObject's current up direction
	inline Vector3D Up() const { return m_up; }

	// Returns the GameObject's scale along the x axis
	inline float GetScaleX() const { return m_scaleX; }
	// Sets the GameObject's scale along the x axis
	void SetScaleX(float scaleX);
	// Adds to the GameObject's scale along the x axis
	void ScaleXby(float amount);

	// Returns the GameObject's scale along the y axis
	inline float GetScaleY() const { return m_scaleY; }
	// Sets the GameObject's scale along the y axis
	void SetScaleY(float scaleY);
	// Adds to the GameObject's scale along the y axis
	void ScaleYby(float amount);

	// Returns the GameObject's scale along the z axis
	inline float GetScaleZ() const { return m_scaleZ; }
	// Sets the GameObject's scale along the z axis
	void SetScaleZ(float scaleZ);
	// Adds to the GameObject's scale along the z axis
	void ScaleZby(float amount);

	// Sets the GameObject's scale along all axis uniformly
	void SetScaleUniform(float amount);
	// Sets the GameObject's scale along all axis
	void SetScale(float scaleX, float scaleY, float scaleZ);
	// Sets the GameObject's scale along all axis using the x, y, and z components of the scale vector
	void SetScale(const Vector3D& scale);
	// Increases the scale of the GameObject in along all axis by the amoutn given
	void ScaleUniform(float amount);
	// Returns a Vector3D with the x scale, y scale, and z scale as the x, y, and z components
	inline Vector3D GetScaleVector() const { return Vector3D(m_scaleX, m_scaleY, m_scaleZ, 0); }

	// Returns the GameObject's current transform matrix calculated from the scale, rotation, and translation
	inline Matrix4x4 GetTransform() const { return m_transform; }
	// Returns the GameObject's current rotation matrix calculated from the rotation amounts
	Matrix4x4 GetRotationMatrix() const;
};

#endif

