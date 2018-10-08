/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef CONTROLLER_H
#define CONTROLLER_H

class ControllerComponentManager;

class ControllerComponent : public Component {
private:

protected:
	friend ControllerComponentManager;
public:
	static const unsigned CACHESIZE = 5;
	static const ComponentType Type = ComponentType::C_Controller;
	virtual ComponentType GetType() const { return Type; }

	ControllerComponent(InfectGUID guid);
	~ControllerComponent() {};

	static Component* CreateInstance(InfectGUID guid) { return new ControllerComponent(guid);  }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	virtual void HandleEvent(Event * p_event);

};

#endif

