/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLightComopnent : public LightBaseComponent
{
public:
	static const ComponentType Type = ComponentType::C_PointLight;
	virtual ComponentType GetType() const { return Type; }

	PointLightComopnent(InfectGUID guid) : LightBaseComponent(guid) {};
	~PointLightComopnent() {};

	static Component* CreateInstance(InfectGUID guid) { return new PointLightComopnent(guid); }
	virtual void Deactivate();
	virtual void Update(float dt) {};
	virtual void Serialize(const json& j) {};
	void Override() {};
};

#endif

