/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void ComponentManagerContainer::Update(float dt)
{
	for each (CM* cm in m_CMngrs) {
		cm->Update(dt);
	}
}

void ComponentManagerContainer::HandleEvent(Event * pEvent)
{
	for each (CM* cm in m_CMngrs) {
		cm->HandleEvent(pEvent);
	}
}

void ComponentManagerContainer::Init() {
	for (CM* mngr : m_CMngrs) {
		mngr->Init();
	}
}

void ComponentManagerContainer::RegisterCompMngr(CM * cMngr)
{
	m_CMngrs.push_back(cMngr);
}

void ComponentManagerContainer::RegisterCompToCompMngr(Component* comp, ComponentType cType) {
	m_CMngrs[(int)cType]->Register(comp);
}