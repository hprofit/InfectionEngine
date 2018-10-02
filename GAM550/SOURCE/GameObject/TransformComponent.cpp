/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

TransformComponent::TransformComponent(InfectGUID guid) :
	Component(guid),
	m_prevPosition(Vector3D()),
	m_position(Vector3D()),
	m_scaleX(1.0f),
	m_scaleY(1.0f),
	m_scaleZ(1.0f),
	m_transform(Matrix4x4()),
	m_pivotOffset(Vector3D()),
	m_lookAt(Vector3D(0.f, 0.f, -1.f, 0.f)),
	m_angleX(0.f), m_angleY(0.f), m_angleZ(0.f)
{
}

void TransformComponent::Deactivate() {
	mp_Parent = nullptr;
}

void TransformComponent::Serialize(const json& j) {
	m_position.x = ParseFloat(j["position"], "x");
	m_position.y = ParseFloat(j["position"], "y");
	m_position.z = ParseFloat(j["position"], "z");

	m_scaleX = ParseFloat(j["scale"], "x");
	m_scaleY = ParseFloat(j["scale"], "y");
	m_scaleZ = ParseFloat(j["scale"], "z");

	m_pivotOffset.x = ParseFloat(j["pivotOffset"], "x");
	m_pivotOffset.y = ParseFloat(j["pivotOffset"], "y");
	m_pivotOffset.z = ParseFloat(j["pivotOffset"], "z");

	m_angleX = ParseFloat(j, "rotation", "x");
	m_angleY = ParseFloat(j, "rotation", "y");
	m_angleZ = ParseFloat(j, "rotation", "z");

	m_IsDirty = true;
}

void TransformComponent::Override(const json& j)
{
	if (ValueExists(j, "position")) {
		m_position.x = ValueExists(j["position"], "x") ? j["position"]["x"] : m_position.x;
		m_position.y = ValueExists(j["position"], "y") ? j["position"]["y"] : m_position.y;
		m_position.z = ValueExists(j["position"], "z") ? j["position"]["z"] : m_position.z;
	}
	if (ValueExists(j, "scale")) {
		m_scaleX = ValueExists(j["scale"], "x") ? j["scale"]["x"] : m_scaleX;
		m_scaleY = ValueExists(j["scale"], "y") ? j["scale"]["y"] : m_scaleY;
		m_scaleZ = ValueExists(j["scale"], "z") ? j["scale"]["z"] : m_scaleZ;
	}
	if (ValueExists(j, "rotation")) {
		m_angleX = ValueExists(j["rotation"], "x") ? j["rotation"]["x"] : m_angleX;
		m_angleY = ValueExists(j["rotation"], "y") ? j["rotation"]["y"] : m_angleY;
		m_angleZ = ValueExists(j["rotation"], "z") ? j["rotation"]["z"] : m_angleZ;
	}

	m_IsDirty = true;
}

void TransformComponent::HandleEvent(Event * p_event) {
	//switch (p_event->Type()) {
	//default:
	//	break;
	//}
}

#pragma region Translation
void TransformComponent::SetPosition(const Vector3D& pos)
{
	m_position = pos;
	m_IsDirty = true;
}

void TransformComponent::Move(const Vector3D& amount)
{
	m_position += amount;
	m_IsDirty = true;
}

void TransformComponent::MoveAlongLookAt(Vector3D & amount)
{
	m_position += (m_rotation * amount);
	m_IsDirty = true;
}
#pragma endregion

#pragma region Rotate
void TransformComponent::SetAngles(float angleX, float angleY, float angleZ)
{
	m_angleX = angleX;
	m_angleY = angleY;
	m_angleZ = angleZ;
	m_IsDirty = true;
}

void TransformComponent::SetAngleX(float angle)
{
	m_angleX = angle;
	m_IsDirty = true;
}

void TransformComponent::SetAngleY(float angle)
{
	m_angleY = angle;
	m_IsDirty = true;
}

void TransformComponent::SetAngleZ(float angle)
{
	m_angleZ = angle;
	m_IsDirty = true;
}

void TransformComponent::SetPivotOffset(Vector3D offset)
{
	m_pivotOffset = offset;
	m_IsDirty = true;
}

void TransformComponent::RotateX(float amount)
{
	m_angleX += amount;
	m_IsDirty = true;
}

void TransformComponent::RotateY(float amount)
{
	m_angleY += amount;
	m_IsDirty = true;
}

void TransformComponent::RotateZ(float amount)
{
	m_angleZ += amount;
	m_IsDirty = true;
}
#pragma endregion

#pragma region Scale
void TransformComponent::SetScaleX(float scaleX)
{
	m_scaleX = scaleX;
	m_IsDirty = true;
}

void TransformComponent::ScaleXby(float amount)
{
	m_scaleX += amount;
	m_IsDirty = true;
}

void TransformComponent::SetScaleY(float scaleY)
{
	m_scaleY = scaleY;
	m_IsDirty = true;
}

void TransformComponent::ScaleYby(float amount)
{
	m_scaleY += amount;
	m_IsDirty = true;
}

void TransformComponent::SetScaleZ(float scaleZ)
{
	m_scaleZ = scaleZ;
	m_IsDirty = true;
}

void TransformComponent::ScaleZby(float amount)
{
	m_scaleZ += amount;
	m_IsDirty = true;
}

void TransformComponent::SetScaleUniform(float amount)
{
	m_scaleZ = m_scaleY = m_scaleZ = amount;
	m_IsDirty = true;
}

void TransformComponent::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_scaleX = scaleX;
	m_scaleY = scaleY;
	m_scaleZ = scaleZ;
	m_IsDirty = true;
}

void TransformComponent::SetScale(const Vector3D& scale)
{
	m_scaleX = scale.x;
	m_scaleY = scale.y;
	m_scaleZ = scale.z;
	m_IsDirty = true;
}

void TransformComponent::ScaleUniform(float amount)
{
	m_scaleX += amount;
	m_scaleY += amount;
	m_scaleZ += amount;
	m_IsDirty = true;
}

Matrix4x4 TransformComponent::GetRotationMatrix() const
{
	return Matrix4x4(
		m_right.x, m_right.y, m_right.z, 0.0f,
		m_up.x, m_up.y, m_up.z, 0.0f,
		-m_lookAt.x, -m_lookAt.y, -m_lookAt.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


#pragma endregion