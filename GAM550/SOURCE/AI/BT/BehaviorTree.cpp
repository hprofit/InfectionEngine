/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

BehaviorTree::BehaviorTree(){

}

BehaviorTree::~BehaviorTree() {

}

void BehaviorTree::Init(Behavior& b, BH_Observer* observer) {
	if (observer != nullptr) {
		b.m_Observer = *observer;
	}
	m_Scheduler.ScheduleBehavior(&b);
}

void BehaviorTree::StopBehavior(Behavior& b, BH_Status res) {
	assert(res != BH_RUNNING);
	b.SetStatus(res);
	// Handle status
	if (b.m_Observer)
		b.m_Observer(res);
}

void BehaviorTree::Tick() {
	if (m_Scheduler.Empty())
		m_Scheduler.ScheduleBehavior(m_pRootNode);
	
	// ending marker
	m_Scheduler.ScheduleBehavior(nullptr);

	while (Step()) { continue; }
}

bool BehaviorTree::Step() {
	Behavior* curBehavior = m_Scheduler.NextBehavior();
	if (curBehavior==nullptr) return false; // this frame's Tick finished

	curBehavior->Tick(); // update current behavior

	// if current behavior terminated, handle through observer
	if (curBehavior->Status() != BH_RUNNING && curBehavior->m_Observer) {
		curBehavior->m_Observer(curBehavior->Status());
	}
	else { // if still running, push to scheduler for next frame
		m_Scheduler.ScheduleBehavior(curBehavior);
	}
	return true;
}


void BehaviorTree::ClearScheduler() {
	m_Scheduler.Clear();
}