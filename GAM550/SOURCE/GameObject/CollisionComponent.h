
#pragma once
#ifndef COLLISION_C_H
#define COLLISION_C_H
class physics::RigidBody;
class CollisionComponentManager;
class CollisionComponent : public Component
{
private:


public:
  friend CollisionComponentManager;

  static const ComponentType Type = ComponentType::C_Collision;
  virtual ComponentType GetType() const { return Type; }

  CollisionComponent(InfectGUID guid);
  ~CollisionComponent() {};
  static Component* CreateInstance(InfectGUID guid) { return new CollisionComponent(guid); }
  virtual void Deactivate();
  virtual void LateInitialize();
  virtual void Update(float dt);
  virtual void LateUpdate(float dt) {};
  virtual void Serialize(const json& j);
  virtual void HandleEvent(Event* pEvent);

  // collision info
  const static unsigned maxContacts = 256;
  physics::Contact contacts[maxContacts];
  physics::CollisionData cData;
  physics::ContactResolver resolver = physics::ContactResolver(maxContacts * 8);

  // collision type
  class Box : public physics::CollisionBox
  {
  public:
    Box();
    ~Box();
    void SetState(float mass, Vector3D position, Vector3D velocity = Vector3D(0, 0, 0), Vector3D rotation = Vector3D(0, 0, 0),
      Vector3D Acceleration = Vector3D(0, 0, 0), Vector3D halfSize_ = Vector3D(1, 1, 1),
      float LinearDamping = 0.95f, float AngularDamping = 0.8f);
  }m_Box;

  class Sphere : public physics::CollisionSphere
  {
  public:
    Sphere();
    ~Sphere();
    void setState(float mass, float radius_, Vector3D position, Vector3D velocity,
      Vector3D Acceleration = Vector3D(0, 0, 0),
      float LinearDamping = 0.99f, float AngularDamping = 0.8f);
  }m_Sphere;

  class Plane : public physics::CollisionPlane
  {
  public:
    void setState(Vector3D direction_, real offset_);
  }m_Plane;
};

#endif

