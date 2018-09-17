/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
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
	void Register(MC m) {
		m_Components.push_back(m);
	}
};

#endif
