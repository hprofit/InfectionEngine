/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef CONTROLLER_COMPONENT_MANAGER_H
#define CONTROLLER_COMPONENT_MANAGER_H

class ControllerComponentManager :
	public ComponentManager<ControllerComponent>
{
protected:
	typedef ControllerComponent* CC;
	
public:
	ControllerComponentManager();
	virtual ~ControllerComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);

	void Init();
};

#endif
