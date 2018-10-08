/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>


ConditionalLoop::ConditionalLoop(BehaviorTree& tree, Condition& cond):pBT(&tree), m_CondiFunc(cond) {

}

ConditionalLoop::~ConditionalLoop() {

}

void ConditionalLoop::OnInitialize() {
	if (m_CondiFunc()) {
		BH_Observer obs = std::bind(&ConditionalLoop::OnChildComplete, this, std::placeholders::_1);
		pBT->Init(*m_pChildBehavior , &obs);
	}
}

BH_Status ConditionalLoop::OnUpdate() {
	return BH_RUNNING;
}
void ConditionalLoop::OnTerminate(BH_Status) {

}

void ConditionalLoop::OnChildComplete(BH_Status) {

}