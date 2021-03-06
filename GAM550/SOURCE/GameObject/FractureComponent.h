
#pragma once
#ifndef FRATURE_C_H
#define FRATURE_C_H
class physics::RigidBody;
class PhysicsManager;
class RigidBodyComponent;
class FractureComponentManager;

class FractureComponent : public Component
{
public:
	//physics::Contact* mp_Contact;
	RigidBodyComponent::Box* mp_Target;
	bool m_Hit = false;

public:
  friend PhysicsManager;
  friend FractureComponentManager;

  static const ComponentType Type = ComponentType::C_Fracture;
  virtual ComponentType GetType() const { return Type; }

  FractureComponent(InfectGUID guid);
  ~FractureComponent() {};
  static Component* CreateInstance(InfectGUID guid) { return new FractureComponent(guid); }
  virtual void Deactivate();
  virtual void LateInitialize();
  virtual void Serialize(const json& j);
  virtual void Override(const json& j);
  virtual void HandleEvent(Event* pEvent);

  void SetFractureHit(bool val) { m_Hit = val; }
  void SetFractureTarget(RigidBodyComponent::Box* val) { mp_Target = val; }
};

#endif

