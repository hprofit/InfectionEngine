/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

class GameObject;
class Component;

class CM : public Subscriber 
{

public:
	CM() {};
	virtual ~CM() {};

	virtual void Update(float dt) = 0;
	virtual void HandleEvent(Event* pEvent) = 0;
	virtual void Init() = 0;
};

template <typename CType>
class ComponentManager :
	public CM
{
protected:
	std::list<CType*> *m_Components;

public:
	ComponentManager() {
		static_assert(std::is_base_of<Component, CType>::value, "CType must inherit from Component class.");
	};
	virtual ~ComponentManager() {};

	virtual void Update(float dt) = 0;
	virtual void HandleEvent(Event* pEvent) = 0;
	virtual void Init() = 0;
};

#endif
