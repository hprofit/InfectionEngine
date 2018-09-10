/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#pragma region Private Methods

void TransformComponent::_UpdateLookAt()
{
	m_rotation = Matrix4x4::Rotate(m_angleZ, Vector3D(0,0,1.0f,0)) *
		Matrix4x4::Rotate(m_angleY, YAXIS) *
		Matrix4x4::Rotate(m_angleX, XAXIS);
	m_lookAt = Vector3D::Normalize(m_rotation * (Vector3D(0, 0, -1.0f, 0)));
	m_lookAt.w = 0;
}

#pragma endregion

TransformComponent::TransformComponent() :
	//Component(ComponentType::C_Transform),
	Component(),
	m_prevPosition(Vector3D()),
	m_position(Vector3D()),
	m_scale(Vector3D()), 
	m_transform(Matrix4x4()),
	m_pivotOffset(Vector3D()),
	m_lookAt(Vector3D(0.f, 0.f, -1.f, 0.f)),
	m_angleX(0.f), m_angleY(0.f), m_angleZ(0.f)
{
}

TransformComponent::~TransformComponent() 
{
}

void TransformComponent::Deactivate() {
	mp_Parent = nullptr;
	m_parent = nullptr;
}

void TransformComponent::Update(float dt) 
{
}

void TransformComponent::LateUpdate(float dt) {
	m_prevPosition = Vector3D(m_transform._03(), m_transform._13(), m_transform._23());

	// TODO: optimization if game object is static save m_Transform somewhere and never calculate matrix again
	Matrix4x4 trans;
	Matrix4x4 scale;
	if (m_parent) {
		trans = Matrix4x4::Translate(m_position + m_parent->GetPosition());
		int xSign = 1, ySign = 1, zSign = 1;
		if (m_scale.x < 0 && m_parent->GetScaleX() < 0) xSign = -1;
		if (m_scale.y < 0 && m_parent->GetScaleY() < 0) ySign = -1;
		if (m_scale.z < 0 && m_parent->GetScaleZ() < 0) zSign = -1;
		scale = Matrix4x4::Scale(m_scale.x*m_parent->GetScaleX()*xSign, m_scale.y*m_parent->GetScaleY()*ySign, m_scale.z*m_parent->GetScaleZ()*zSign);
	}
	else {
		trans = Matrix4x4::Translate(m_position);
		scale = Matrix4x4::Scale(m_scale.x, m_scale.y, m_scale.z);
	}
	
	Matrix4x4 rot(Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS));

	// TODO: Optimization, if pivot offset is zero do not create or multiply this component
	Matrix4x4 pivotOffset(Matrix4x4::Translate(m_pivotOffset));

	m_transform = trans*rot*scale*pivotOffset;
	m_worldPosition = Vector3D(m_transform._03(), m_transform._13(), m_transform._23());
	_UpdateLookAt();
}

void TransformComponent::Serialize(const json& j) {
	//m_is2d = ValueExists(j, "2D") ? ParseBool(j, "2D") : true;
	//m_angleX = ParseFloat(j, "rotation", "x");
	//m_angleY = ParseFloat(j, "rotation", "y");
	//m_angleZ = ParseFloat(j, "rotation", "z");

	//m_position.x = ParseFloat(j["position"], "x");
	//m_position.y = ParseFloat(j["position"], "y");
	//m_position.z = ParseFloat(j["position"], "z");

	//m_scale.x = ParseFloat(j["scale"], "x");
	//m_scale.y = ParseFloat(j["scale"], "y");
	//m_scale.z = ParseFloat(j["scale"], "z");

	//m_pivotOffset.x = ParseFloat(j["pivotOffset"], "x");
	//m_pivotOffset.y = ParseFloat(j["pivotOffset"], "y");
	//m_pivotOffset.z = ParseFloat(j["pivotOffset"], "z");

	//_UpdateLookAt();
}

void TransformComponent::Override()
{
	//if (ValueExists(j, "position")) {
	//	m_position.x = ValueExists(j["position"], "x") ? j["position"]["x"] : m_position.x;
	//	m_position.y = ValueExists(j["position"], "y") ? j["position"]["y"] : m_position.y;
	//	m_position.z = ValueExists(j["position"], "z") ? j["position"]["z"] : m_position.z;
	//}
	//if (ValueExists(j, "scale")) {
	//	m_scale.x = ValueExists(j["scale"], "x") ? j["scale"]["x"] : m_scale.x;
	//	m_scale.y = ValueExists(j["scale"], "y") ? j["scale"]["y"] : m_scale.y;
	//	m_scale.z = ValueExists(j["scale"], "z") ? j["scale"]["z"] : m_scale.z;
	//}
	//if (ValueExists(j, "rotation")) {
	//	m_angleX = ValueExists(j["rotation"], "x") ? j["rotation"]["x"] : m_angleX;
	//	m_angleY = ValueExists(j["rotation"], "y") ? j["rotation"]["y"] : m_angleY;
	//	m_angleZ = ValueExists(j["rotation"], "z") ? j["rotation"]["z"] : m_angleZ;
	//}

	_UpdateLookAt();
}

void TransformComponent::HandleEvent(Event * p_event) {
	switch (p_event->Type()) {
	default:
		break;
	}
}

bool TransformComponent::operator<(const TransformComponent & other) const
{
	return m_position.y > other.m_position.y;
}

#pragma region Translation
Vector3D TransformComponent::GetPosition() const
{
	if (m_parent)
		return m_transform.GetTranslate();
	else
		return m_position;
}

Vector3D TransformComponent::GetLocalPosition() const
{
	return m_position;
}

void TransformComponent::SetPosition(const Vector3D& pos)
{
	m_position = pos;
}

void TransformComponent::Move(const Vector3D& amount)
{
	m_position += amount;
}

void TransformComponent::MoveAlongLookAt(Vector3D & amount)
{
	m_position += (m_rotation * amount);
}
#pragma endregion

#pragma region Rotate
void TransformComponent::SetAngles(float angleX, float angleY, float angleZ)
{
	m_angleX = angleX;
	m_angleY = angleY;
	m_angleZ = angleZ;
	_UpdateLookAt();
}

float TransformComponent::GetAngleX() const
{
	return m_angleX;// +(m_parentTransform ? m_parentTransform->GetAngleX() : .0f);
}

void TransformComponent::SetAngleX(float angle)
{
	m_angleX = angle;
	_UpdateLookAt();
}

float TransformComponent::GetAngleY() const
{
	return m_angleY;// +(m_parentTransform ? m_parentTransform->GetAngleY() : .0f);
}

void TransformComponent::SetAngleY(float angle)
{
	m_angleY = angle;
	_UpdateLookAt();
}

float TransformComponent::GetAngleZ() const
{
	return m_angleZ;// +(m_parentTransform ? m_parentTransform->GetAngleZ() : .0f);
}

void TransformComponent::SetAngleZ(float angle)
{
	m_angleZ = angle;
	_UpdateLookAt();
}

void TransformComponent::RotateX(float amount)
{
	m_angleX += amount;
	_UpdateLookAt();
}

void TransformComponent::RotateY(float amount)
{
	m_angleY += amount;
	_UpdateLookAt();
}

void TransformComponent::RotateZ(float amount)
{
	m_angleZ += amount;
	_UpdateLookAt();
}

Vector3D TransformComponent::GetRotVector() const
{
	return Vector3D(m_angleX, m_angleY, m_angleZ);
}

Vector3D TransformComponent::Forward() const
{
	return m_lookAt;
}

Vector3D TransformComponent::Right() const
{
	Vector3D up = Vector3D(0,1,0,0);
	Vector3D lCrossR = Vector3D::Cross(m_lookAt, up);
	float lenLCrossR = lCrossR.Length();
	return lenLCrossR != 0.0f ? lCrossR *
		(1.0f / lenLCrossR) : Vector3D(0, 0, 1, 0);
}

Vector3D TransformComponent::Up() const
{
	return Vector3D::Cross(Forward()*-1, Right());
}

Vector3D TransformComponent::LookAt() const
{
	return m_lookAt;
}

float TransformComponent::GetParentScaleX() {
	if (m_parent) {
		return m_parent->GetScaleX();
	}
	else return 0;
}

float TransformComponent::GetParentScaleY() {
	if (m_parent) {
		return m_parent->GetScaleY();
	}
	else return 0;
}
#pragma endregion

#pragma region Scale
float TransformComponent::GetScaleX() const
{
	return m_scale.x;// * (m_parentTransform ? m_parentTransform->GetScaleX() : 1.f);
}

void TransformComponent::SetScaleX(float scaleX)
{
	m_scale.x = scaleX;
}

void TransformComponent::ScaleXby(float amount)
{
	m_scale.x += amount;
}

float TransformComponent::GetScaleY() const
{
	return m_scale.y;// * (m_parentTransform ? m_parentTransform->GetScaleY() : 1.f);
}

void TransformComponent::SetScaleY(float scaleY)
{
	m_scale.y = scaleY;
}

void TransformComponent::ScaleYby(float amount)
{
	m_scale.y += amount;
}

float TransformComponent::GetScaleZ() const
{
	return m_scale.z;// *(m_parentTransform ? m_parentTransform->GetScaleY() : 1.f);
}

void TransformComponent::SetScaleZ(float scaleZ)
{
	m_scale.z = scaleZ;
}

void TransformComponent::ScaleZby(float amount)
{
	m_scale.z += amount;
}

void TransformComponent::SetScaleUniform(float amount)
{
	m_scale.x = m_scale.y = m_scale.z = amount;
}

void TransformComponent::SetScale(float scaleX, float scaleY, float scaleZ)
{
	m_scale.x = scaleX;
	m_scale.y = scaleY;
	m_scale.z = scaleZ;
}

void TransformComponent::SetScale(const Vector3D& scale)
{
	m_scale = scale;
}

void TransformComponent::ScaleUniform(float amount)
{
	m_scale += Vector3D(amount, amount, amount);
}

Vector3D TransformComponent::GetScaleVector() const
{
	return m_scale;
}

#pragma endregion

Matrix4x4 TransformComponent::GetTransform() const
{
	return m_transform;
}

Matrix4x4 TransformComponent::GetTransformAfterOffset(const Vector3D & offset) const
{
	//Matrix4x4 trans;
	//if (m_parent)
	//	trans = Matrix4x4::Translate(m_position + offset + m_parent->GetPosition());
	//else
	//	trans = Matrix4x4::Translate(m_position + offset);

	//Matrix4x4 scale(Matrix4x4::Scale(m_scale.x, m_scale.y, m_scale.z));
	//Matrix4x4 rot(Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS));

	//// TODO: Optimization, if pivot offset is zero do not create or multiply this component
	//Matrix4x4 pivotOffset(Matrix4x4::Translate(m_pivotOffset));

	//return trans*rot*scale*pivotOffset;
	return m_transform;
}

Matrix4x4 TransformComponent::TransformWithOffsetAndScale(const Vector3D & offset, const float & scaleX, const float & scaleY, const float& scaleZ) const
{
	//Matrix4x4 trans;
	//if (m_parent)
	//	trans = Matrix4x4::Translate(m_position + offset + m_parent->GetPosition());
	//else
	//	trans = Matrix4x4::Translate(m_position + offset);

	//Matrix4x4 scale(Matrix4x4::Scale(scaleX, scaleY, scaleZ));
	//Matrix4x4 rot(Matrix4x4::Rotate(m_angleX, XAXIS) * Matrix4x4::Rotate(m_angleY, YAXIS) * Matrix4x4::Rotate(m_angleZ, ZAXIS));

	//Matrix4x4 pivotOffset(Matrix4x4::Translate(m_pivotOffset));

	//return trans*rot*scale*pivotOffset;
	return m_transform;
}
