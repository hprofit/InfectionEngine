/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;

class Component : 
	public Subscriber
{
protected:
	
	GameObject *mp_Parent;
public:
	static const ComponentType Type = ComponentType::NUM_COMPONENTS;

	/*Component(ComponentType _type) : m_type(_type) {};*/
	Component() {};
	virtual ~Component() {};
	virtual void LateInitialize() {};
	virtual void Update(float dt) = 0;
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j) = 0;
	virtual void Deactivate() {};
	//virtual void Override(const json& j) {};
	virtual void Override() {};
	virtual void HandleEvent(Event* pEvent) {}

	virtual ComponentType GetType() const { return Type; }
	inline GameObject* Parent() { return mp_Parent; }
	inline void SetParent(GameObject* parent) { mp_Parent = parent; }
};

#endif
