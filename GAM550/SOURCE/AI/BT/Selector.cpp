/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void Selector::OnInitialize() {
	m_currentNode = m_childBehaviors.begin();
	BH_Observer obs = std::bind(&Selector::OnChildComplete, this, std::placeholders::_1);
	pBT->Init(**m_currentNode, &obs);
}

BH_Status Selector::OnUpdate() {
	return BH_RUNNING;
}

void Selector::OnTerminate(BH_Status status) {

}

void Selector::OnChildComplete(BH_Status status) {
	Behavior& child = **m_currentNode;
	switch (child.Status()) {
	case BH_SUCCESS:
		pBT->StopBehavior(*this, BH_SUCCESS);
		return;
	case BH_SUSPENDED:
		break;
	case BH_FAILURE:
		if (++m_currentNode == m_childBehaviors.end()) {
			pBT->StopBehavior(*this, BH_FAILURE);
		}
		else {
			BH_Observer obs = std::bind(&Selector::OnChildComplete, this, std::placeholders::_1);
			pBT->Init(**m_currentNode, &obs);
		}
		return;
	}

	//if (child.Status() == BH_SUCCESS) {
	//	pBT->StopBehavior(*this, BH_SUCCESS);
	//	return;
	//}
	//assert(child.Status() == BH_FAILURE);
	//if (++m_currentNode == m_childBehaviors.end()) {
	//	pBT->StopBehavior(*this, BH_FAILURE);
	//}
	//else {
	//	BH_Observer obs = std::bind(&Selector::OnChildComplete, this, std::placeholders::_1);
	//	pBT->Init(**m_currentNode, &obs);
	//}
}