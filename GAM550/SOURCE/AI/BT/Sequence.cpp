/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Sequence::Sequence(BehaviorTree& tree, int size): pBT(&tree){
	m_childBehaviors.reserve(size);
}

Sequence::~Sequence() {

}

void Sequence::OnInitialize() {
	m_currentNode = m_childBehaviors.begin();
	BH_Observer obs = std::bind(&Sequence::OnChildComplete, this, std::placeholders::_1);
	pBT->Init(**m_currentNode, &obs);
}

BH_Status Sequence::OnUpdate() {
	return BH_RUNNING;
}

void Sequence::OnTerminate(BH_Status status) {

}

void Sequence::OnChildComplete(BH_Status status) {
	Behavior& child = **m_currentNode;
	if (child.Status() == BH_FAILURE) {
		pBT->StopBehavior(*this, BH_FAILURE);
		return;
	}
	assert(child.Status() == BH_SUCCESS);
	if (++m_currentNode == m_childBehaviors.end()) {
		pBT->StopBehavior(*this, BH_SUCCESS);
	}
	else {
		BH_Observer obs = std::bind(&Sequence::OnChildComplete, this, std::placeholders::_1);
		pBT->Init(**m_currentNode, &obs);
	}
}