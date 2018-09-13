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
	D3DXCOLOR m_color;
	float m_distance;	// How far before the light falls off entirely
	float m_a, m_b;		// Light fall off  (1 / (1 + a * d + b^2 * d) )

public:
	static const ComponentType Type = ComponentType::C_Transform;
	virtual ComponentType GetType() const { return Type; }

	LightBaseComponent(InfectGUID guid) : Component(guid) {};
	~LightBaseComponent() {};

	virtual void LateInitialize();

	inline FLOAT Red() const { return m_color.r; }
	inline FLOAT Green() const { return m_color.g; }
	inline FLOAT Blue() const { return m_color.b; }
	inline FLOAT Alpha() const { return m_color.a; }
	inline D3DXCOLOR GetColor() const { return m_color; }
	inline void SetColor(const D3DXCOLOR& color) { m_color = color; }
	inline float Distance() const { return m_distance; }
	inline float LightA() const { return m_a; }
	inline float LightB() const { return m_b; }
	inline void SetFallOff(float a, float b) { m_a = a; m_b = b; }
};

#endif

