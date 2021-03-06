/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef AI_STATE_H
#define AI_STATE_H

class Behavior;
class BehaviorTree;

// First Layer of Agent's AI - FSM
// Acting blackboard of BT that runs the state logic
class AI_State: public Subscriber{
protected:
	AIStateType m_StateType;
	Behavior* m_pRootNode;
	BehaviorTree* m_pBT;
public:	
	static const AIStateType Type = AIStateType::NUM_AI_STATES;
	virtual AIStateType GetType() const { return Type; }

	AI_State(AIStateType _type): m_StateType(_type){}
	virtual ~AI_State(){}

	virtual void OnEnter() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnExit() = 0;
	virtual void HandleEvent(Event* pEvent) = 0;

	void SetBehaviorTree(BehaviorTree* tree) { m_pBT = tree; }
	BehaviorTree* GetBehaviorTree() { return m_pBT; }

	BrainComponent *pAgent;
};

#endif