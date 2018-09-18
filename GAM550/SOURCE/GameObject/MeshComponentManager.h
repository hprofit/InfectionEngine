/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef MESH_COMPONENT_MANAGER_H
#define MESH_COMPONENT_MANAGER_H

class MeshComponentManager :
	public ComponentManager<MeshComponent>
{
protected:
	typedef MeshComponent* MC;

public:
	MeshComponentManager() {};
	virtual ~MeshComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);


	// TODO: Get rid of this
	void Register(Component* m) {
		m_Components.push_back(static_cast<MC>(m));
	}
	void Unregister(Component* t) {}
	void Init();
};

#endif
