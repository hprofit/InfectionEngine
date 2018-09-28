/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

class PointLightComponentManager;

class PointLightComponent : public LightBaseComponent
{
protected:
	// TODO: Add primitive Mesh member

public:
	friend PointLightComponentManager;

	static const unsigned CACHESIZE = 300;
	static const ComponentType Type = ComponentType::C_PointLight;
	virtual ComponentType GetType() const { return Type; }

	PointLightComponent(InfectGUID guid) : LightBaseComponent(guid) {};
	~PointLightComponent() {};

	static Component* CreateInstance(InfectGUID guid) { return new PointLightComponent(guid); }
	virtual void Deactivate();
	virtual void Update(float dt) {};
	virtual void Serialize(const json& j) {};
	void Override() {};
};

#endif

