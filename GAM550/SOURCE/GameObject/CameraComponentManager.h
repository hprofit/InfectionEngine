/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef CAMERA_COMPONENT_MANAGER_H
#define CAMERA_COMPONENT_MANAGER_H

class CameraComponentManager :
	public ComponentManager<CameraComponent>
{
protected:
	typedef CameraComponent* CC;

	void _CalcViewMatrix(CC comp);
public:
	CameraComponentManager() {};
	virtual ~CameraComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);


	// TODO: Get rid of this
	void Register(CC t) {
		m_Components.push_back(t);
	}
};

#endif
