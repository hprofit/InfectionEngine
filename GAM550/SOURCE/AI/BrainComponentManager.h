/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef BRAIN_COMPONENT_MANAGER_H
#define BRAIN_COMPONENT_MANAGER_H

class BrainComponentManager :
	public ComponentManager<BrainComponent>
{
protected:
	typedef BrainComponent* BC;
	
public:
	BrainComponentManager();
	virtual ~BrainComponentManager() {};


	virtual void Update(float dt);
	virtual void HandleEvent(Event* pEvent);

	void Init();
};

#endif
