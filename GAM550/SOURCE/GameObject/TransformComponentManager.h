/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef TRANSFORM_COMPONENT_MANAGER_H
#define TRANSFORM_COMPONENT_MANAGER_H

class TransformComponentManager :
	public ComponentManager<TransformComponent>
{
protected:
	typedef TransformComponent* TC;

	void _UpdateLookAt(TC tComp);
	void _UpdateRightUp(TC tComp);
	void _UpdateTransform(TC tComp);

public:
	TransformComponentManager() {};
	virtual ~TransformComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);
};

#endif
