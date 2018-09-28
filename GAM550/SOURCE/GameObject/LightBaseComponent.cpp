#include "Stdafx.h"

LightBaseComponent::LightBaseComponent(InfectGUID guid) :
	Component(guid),
	m_color(Color(1, 1, 1, 1)),
	m_distance(1.0f),
	m_a(0.25f), m_b(0.1f),
	m_intensity(1.0f)
{
}

void LightBaseComponent::LateInitialize()
{
	INFECT_GOM.RegisterLight(mp_Parent);
}

void LightBaseComponent::Serialize(const json & j)
{
	if (ValueExists(j, "color"))
		m_color = ParseColor(j, "color");
	if (ValueExists(j, "distance"))
		m_distance = ParseFloat(j, "distance");
	if (ValueExists(j, "a"))
		m_a = ParseFloat(j, "a");
	if (ValueExists(j, "b"))
		m_b = ParseFloat(j, "b");
	if (ValueExists(j, "intensity"))
		m_intensity = ParseFloat(j, "intensity");

	m_IsDirty = true;
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
