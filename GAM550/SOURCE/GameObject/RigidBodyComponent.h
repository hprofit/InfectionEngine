
#pragma once
#ifndef RIGIDBODY_C_H
#define RIGIDBODY_C_H
class physics::RigidBody;
class RigidBodyComponentManager;
class RigidBodyComponent : public Component
{
private:

	
public:
  friend RigidBodyComponentManager;

	physics::RigidBody m_RigidBody;

  static const ComponentType Type = ComponentType::C_RigidBody;
	virtual ComponentType GetType() const { return Type; }

	RigidBodyComponent(InfectGUID guid);
	~RigidBodyComponent() {};
	static Component* CreateInstance(InfectGUID guid) { return new RigidBodyComponent(guid); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt) {};
	virtual void Serialize(const json& j);
	virtual void HandleEvent(Event* pEvent);
};

#endif

