/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void ComponentManagerContainer::Update(float dt)
{
	//if (!m_isQuitSeq) {
		for each (CM* cm in m_CMngrs) { 
			if (cm)
				cm->Update(dt);
		}
	//}
}

void ComponentManagerContainer::HandleEvent(Event * pEvent)
{	
	/*if (pEvent->Type()== EVENT_WINDOW_CLOSED){
		m_isQuitSeq = true;
	}*/
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
