#include "Stdafx.h"

void LightBaseComponent::LateInitialize()
{
	INFECT_GOM.RegisterLight(mp_Parent);
}

void LightBaseComponent::SetColor(const Color & color)
{
	m_color = color;
	m_IsDirty = true;
}

void LightBaseComponent::SetDistance(float distance)
{
	m_distance = distance;
	m_IsDirty = true;
}

void LightBaseComponent::SetFallOff(float a, float b)
{
	m_a = a;
	m_b = b;
	m_IsDirty = true;
}

void LightBaseComponent::SetIntensity(float intensity)
{
	m_intensity = intensity;
	m_IsDirty = true;
}
