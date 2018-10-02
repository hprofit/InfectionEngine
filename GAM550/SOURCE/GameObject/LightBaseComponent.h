/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef LIGHT_BASE_H
#define LIGHT_BASE_H

class LightBaseComponent : public Component
{
protected:
	Color m_color;
	float m_distance;	// How far before the light falls off entirely
	float m_a, m_b;		// Light fall off  (1 / (1 + a * d + b^2 * d) )
	float m_intensity;	// Intensity of the light

public:
	static const ComponentType Type = ComponentType::C_Transform;
	virtual ComponentType GetType() const { return Type; }

	LightBaseComponent(InfectGUID guid);
	~LightBaseComponent() {};

	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	inline FLOAT Red() const { return m_color.r; }
	inline FLOAT Green() const { return m_color.g; }
	inline FLOAT Blue() const { return m_color.b; }
	inline FLOAT Alpha() const { return m_color.a; }
	inline Color GetColor() const { return m_color; }
	void SetColor(const Color& color);

	inline float Distance() const { return m_distance; }
	void SetDistance(float distance);

	inline float LightA() const { return m_a; }
	inline float LightB() const { return m_b; }
	void SetFallOff(float a, float b);

	inline float Intensity() const { return m_intensity; }
	void SetIntensity(float intensity);
};

#endif

