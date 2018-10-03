
#pragma once
#ifndef COLLISIONTRIGGER_C_H
#define COLLISIONTRIGGER_C_H
class CollisionComponent;
class CollisionTriggerComponentManager;
class CollisionTriggerComponent : public Component
{
public:
  friend CollisionTriggerComponentManager;


  static const ComponentType Type = ComponentType::C_CollisionTrigger;
  virtual ComponentType GetType() const { return Type; }

  CollisionTriggerComponent(InfectGUID guid);
  ~CollisionTriggerComponent() {};
  static Component* CreateInstance(InfectGUID guid) { return new CollisionTriggerComponent(guid); }
  virtual void Deactivate();
  virtual void LateInitialize() {};
  virtual void Update(float dt);
  virtual void LateUpdate(float dt) {};
  virtual void Serialize(const json& j);
  virtual void HandleEvent(Event* pEvent);


  std::list<CollisionComponent::Box *> m_BoxPool;
  std::list<CollisionComponent::Sphere *> m_SpherePool;
  std::list<CollisionComponent::Plane *> m_PlanePool;

  void CollisionTrigger();

  void AddToBoxPool(CollisionComponent::Box *box);

  void AddToSpherePool(CollisionComponent::Sphere *sphere);

  void AddToPlanePool(CollisionComponent::Plane *plane);


  void SetCollisionData(real friction_ = (real)0.9, real restitution_ = (real)0.1, real tolerance_ = (real)0.1);

private:
  real m_CollisionData[3];
  // collision info
  const static unsigned maxContacts = 256;
  physics::Contact contacts[maxContacts];
  physics::CollisionData cData;
  physics::ContactResolver resolver = physics::ContactResolver(maxContacts * 8);
};


#endif