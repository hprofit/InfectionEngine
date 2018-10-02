/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef COMPONENT_MANAGER_CONTAINER_H
#define COMPONENT_MANAGER_CONTAINER_H

class GameObject;
class CM;

class ComponentManagerContainer :
	public Subscriber
{
private:
	//bool m_isQuitSeq = false;
protected:
	std::vector<CM*> m_CMngrs;

public:
	ComponentManagerContainer() {};
	~ComponentManagerContainer() {};

	void Update(float dt);
	void HandleEvent(Event* pEvent);
	void Init();
	void RegisterCompMngr(CM* cMngr);

	// TODO: Get rid of this
	//CM* GetCM(int idx) { return m_CMngrs[idx]; }
};

#endif