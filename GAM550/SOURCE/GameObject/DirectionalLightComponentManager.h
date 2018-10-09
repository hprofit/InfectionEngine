/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef DIRECTIONAL_LIGHT_COMPONENT_MANAGER_H
#define DIRECTIONAL_LIGHT_COMPONENT_MANAGER_H

class DirectionalLightComponentManager :
	public ComponentManager<DirectionalLightComponent>
{
protected:
	typedef DirectionalLightComponent* DLC;

	void _CalcViewPerspMatrix(DLC comp);
public:
	DirectionalLightComponentManager();
	virtual ~DirectionalLightComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);

	void Init();
};

#endif
