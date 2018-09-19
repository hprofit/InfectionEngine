/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef POINT_LIGHT_COMPONENT_MANAGER_H
#define POINT_LIGHT_COMPONENT_MANAGER_H

class PointLightComponentManager :
	public ComponentManager<PointLightComponent>
{
protected:
	typedef PointLightComponent* PLC;

public:
	PointLightComponentManager();
	virtual ~PointLightComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);

	void Init();
};

#endif
