/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

DirectionalLightComponent::DirectionalLightComponent(InfectGUID guid) :
	LightBaseComponent(guid, true),
	m_Far(1000000.f), m_FoV(90.f),
	m_ViewPerspMat(Matrix4x4()),
	mp_LightRenderTarget(nullptr)
{
}

void DirectionalLightComponent::Deactivate()
{
	mp_Parent = nullptr;
	
	if (mp_LightRenderTarget) {
		mp_LightRenderTarget->Release();
		mp_LightRenderTarget = nullptr;
	}
}

void DirectionalLightComponent::Serialize(const json & j)
{
	LightBaseComponent::Serialize(j);
	m_FoV = ParseFloat(j, "fov");
	if (ValueExists(j, "far"))
		m_Far = ParseFloat(j, "far");

	UINT height, width;
	height = ParseInt(j, "height");
	width = ParseInt(j, "width");

	m_AspectRatio = FLOAT(width) / FLOAT(height);

	mp_LightRenderTarget = new RenderTarget(width, height, 2);
	mp_LightRenderTarget->Initialize(INFECT_RENDERER.Device());

	m_IsDirty = true;
}

void DirectionalLightComponent::Override(const json & j)
{
	LightBaseComponent::Serialize(j);
	if (ValueExists(j, "fov"))
		m_FoV = ParseFloat(j, "fov");
	if (ValueExists(j, "far"))
		m_Far = ParseFloat(j, "far");

	if (ValueExists(j, "height") || ValueExists(j, "width")) {
		if (mp_LightRenderTarget) {
			mp_LightRenderTarget->Release();
			mp_LightRenderTarget = nullptr;
		}

		UINT height, width;
		height = ParseInt(j, "height");
		width = ParseInt(j, "width");

		m_AspectRatio = FLOAT(width) / FLOAT(height);

		mp_LightRenderTarget = new RenderTarget(width, height, 2);
		mp_LightRenderTarget->Initialize(INFECT_RENDERER.Device());
	}

	m_IsDirty = true;
}

void DirectionalLightComponent::SetFar(FLOAT _far)
{
	m_Far = _far;
	m_IsDirty = true;
}

void DirectionalLightComponent::SetFoV(FLOAT _fov)
{
	m_FoV = _fov;
	m_IsDirty = true;
}

void DirectionalLightComponent::ClearRenderTarget()
{
	mp_LightRenderTarget->ClearRenderTarget(INFECT_RENDERER.DeviceContext());
}

void DirectionalLightComponent::BindRenderTarget()
{
	mp_LightRenderTarget->BindRenderTarget(INFECT_RENDERER.DeviceContext());
}
