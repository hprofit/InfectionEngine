/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef BRAIN_H
#define BRAIN_H

class BrainComponentManager;
class AI_State;

class BrainComponent : public Component {
private:
	Vector3D m_InitPos;
	float m_detectionRadius;
	float m_IdleMovementRadius;
	AIStateType m_currentState;
	AIStateType m_previousState;
	AI_State* m_states[NUM_AI_STATES];
protected:
	friend BrainComponentManager;
public:
	static const unsigned CACHESIZE = 300;
	static const ComponentType Type = ComponentType::C_Brain;
	virtual ComponentType GetType() const { return Type; }

	BrainComponent(InfectGUID guid);
	~BrainComponent() {};

	static Component* CreateInstance(InfectGUID guid) { return new BrainComponent(guid);  }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);

	virtual void HandleEvent(Event * p_event);

	bool isDestinationReached(const Vector3D& targetPos);	// true if targetPos is reached
	void GetRandomPos(Vector3D& targetPos);					// set targetPos to random position within the m_IdleMovementRadius
	void MoveTo(const Vector3D& targetPos);					// move TransformComponent every frame to targetPos
};

#endif

