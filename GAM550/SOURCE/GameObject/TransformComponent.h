/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef TRANSFORM_H
#define TRANSFORM_H

class TransformComponent : public Component {
private:
	Vector3D m_position;
	Vector3D m_prevPosition;
	Vector3D m_worldPosition;
	Vector3D m_scale;
	Vector3D m_pivotOffset;
	Matrix4x4 m_transform;
	Matrix4x4 m_rotation;
	Vector3D m_lookAt;
	TransformComponent* m_parent;
	float m_angleX, m_angleY, m_angleZ;
	bool m_is2d;

	void _UpdateLookAt();
public:
	TransformComponent();
	~TransformComponent();

	static Component* CreateInstance() { return new TransformComponent();  }
	virtual void Deactivate();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Serialize();
	virtual void Override();

	virtual void HandleEvent(Event * p_event);

	bool operator<(const TransformComponent& other) const;
	bool IsParented() { return m_parent; }

	void SetPosition(const Vector3D& pos);
	Vector3D GetPosition() const;
	Vector3D GetLocalPosition() const;
	void Move(const Vector3D& amount);
	void MoveAlongLookAt(Vector3D& amount);
	Vector3D GetMovement() const { return m_worldPosition - m_prevPosition; }

	void SetAngles(float angleX, float angleY, float angleZ);
	float GetAngleX() const;
	void SetAngleX(float angle);
	float GetAngleY() const;
	void SetAngleY(float angle);
	float GetAngleZ() const;
	void SetAngleZ(float angle);
	float GetParentScaleX();
	float GetParentScaleY();

	void SetPivotOffset(float x, float y, float z) {
		m_pivotOffset.x = x;
		m_pivotOffset.y = y;
		m_pivotOffset.z = z;
	}

	float GetPivotOffsetX() { return m_pivotOffset.x; }
	float GetPivotOffsetY() { return m_pivotOffset.y; }

	void RotateX(float amount);
	void RotateY(float amount);
	void RotateZ(float amount);
	Vector3D GetRotVector() const;

	Vector3D Forward() const;
	Vector3D Right() const;
	Vector3D Up() const;

	Vector3D LookAt() const;

	float GetScaleX() const;
	void SetScaleX(float scaleX);
	void ScaleXby(float amount);

	float GetScaleY() const;
	void SetScaleY(float scaleY);
	void ScaleYby(float amount);

	float GetScaleZ() const;
	void SetScaleZ(float scaleZ);
	void ScaleZby(float amount);
	void SetParent(TransformComponent* pParent) { m_parent = pParent; }

	void SetScaleUniform(float amount);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	void SetScale(const Vector3D& scale);
	void ScaleUniform(float amount);
	Vector3D GetScaleVector() const;

	Matrix4x4 GetTransform() const;
	Matrix4x4 GetTransformAfterOffset(const Vector3D& offset) const;
	Matrix4x4 TransformWithOffsetAndScale(const Vector3D& offset, const float& scaleX, const float& scaleY, const float& scaleZ = 1.0f) const;
};

#endif

