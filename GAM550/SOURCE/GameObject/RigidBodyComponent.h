

#pragma once
#ifndef RIGIDBODY_C_H
#define RIGIDBODY_C_H
class physics::RigidBody;
class RigidBodyComponentManager;
class PhysicsManager;
class RigidBodyComponent : public Component
{
private:
	Vector3D m_position;
	Vector3D m_halfSize;
	Vector3D m_rotation;

	enum RigidBodyType
	{
		BoxRigidBody,
		SphereRigidBody,
		PlaneRigidBody
	} cur_type;	
	
	

public:
  friend RigidBodyComponentManager;
  friend PhysicsManager;

	physics::RigidBody m_RigidBody;

  static const ComponentType Type = ComponentType::C_RigidBody;
	virtual ComponentType GetType() const { return Type; }

	RigidBodyComponent(InfectGUID guid);
	~RigidBodyComponent() {};
	static Component* CreateInstance(InfectGUID guid) { return new RigidBodyComponent(guid); }
	virtual void Deactivate();
	virtual void LateInitialize();
	virtual void Serialize(const json& j);
	virtual void Override(const json& j);
	virtual void HandleEvent(Event* pEvent);


  // collision type
  class Box : public physics::CollisionBox
  {
  public:
    Box();
    ~Box();
    void SetState(float mass, Vector3D position, Vector3D velocity = Vector3D(0, 0, 0), Vector3D rotation = Vector3D(0, 0, 0),
      Vector3D Acceleration = Vector3D(0, 0, 0), Vector3D halfSize_ = Vector3D(1, 1, 1),
      float LinearDamping = 0.95f, float AngularDamping = 0.8f);

    void calculateMassProperties(real invDensity);

	// use for fractureComponent 
	bool m_Breaker = false;
	bool m_CanBeBroken = false;
	bool m_hit = false;
  };
  
  class Sphere : public physics::CollisionSphere
  {
  public:
    Sphere();
    ~Sphere();
    void setState(float mass, float radius_, Vector3D position, Vector3D velocity,
      Vector3D Acceleration = Vector3D(0, 0, 0),
      float LinearDamping = 0.99f, float AngularDamping = 0.8f);

	// use for fractureComponent 
	bool m_Breaker = false;
	bool m_CanBeBroken = false;
	bool m_hit = false;
  };

  class Plane : public physics::CollisionPlane
  {
  public:
    void setState(Vector3D direction_, real offset_);

	// use for fractureComponent 
	bool m_Breaker = false;
	bool m_CanBeBroken = false;
	bool m_hit = false;
  };

  Box *GetBoxPointer() { return mp_newbox; }
  Sphere *GetSpherePointer() { return mp_newSphere; }
  Plane *GetPlanePointer() { return mp_newPlane; }
//private:
	Box *mp_newbox;
	Sphere *mp_newSphere;
	Plane *mp_newPlane;
};

#endif

