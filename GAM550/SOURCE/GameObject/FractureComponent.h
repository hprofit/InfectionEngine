
#pragma once
#ifndef FRATURE_C_H
#define FRATURE_C_H
class physics::RigidBody;
class PhysicsManager;
class RigidBodyComponent;
class FractureComponentManager;
class FractureComponent : public Component
{
private:
  void divideBlock(const physics::Contact& contact,
    RigidBodyComponent::Box* target, RigidBodyComponent::Box* blocks);

public:
  friend PhysicsManager;
  friend FractureComponentManager;

  physics::RigidBody m_RigidBody;

  static const ComponentType Type = ComponentType::C_Fracture;
  virtual ComponentType GetType() const { return Type; }

  FractureComponent(InfectGUID guid);
  ~FractureComponent() {};
  static Component* CreateInstance(InfectGUID guid) { return new FractureComponent(guid); }
  virtual void Deactivate();
  virtual void LateInitialize();
  virtual void Update(float dt);
  virtual void LateUpdate(float dt) {};
  virtual void Serialize(const json& j);
  virtual void HandleEvent(Event* pEvent);

  physics::Contact* contact_;
  RigidBodyComponent::Box* target;
  bool hit = false;
};

#endif

