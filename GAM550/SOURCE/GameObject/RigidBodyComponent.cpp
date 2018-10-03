#include <Stdafx.h>

RigidBodyComponent::RigidBodyComponent(InfectGUID guid) :
	Component(guid)
{
}

void RigidBodyComponent::Deactivate() {
	mp_Parent = nullptr;
}

void RigidBodyComponent::Serialize(const json& j)
{

}

void RigidBodyComponent::HandleEvent(Event * pEvent)
{
	switch (pEvent->Type()) {
	default:
		break;
	}
}

void RigidBodyComponent::LateInitialize()
{
	//FECT_GOM.RegisterCamera(mp_Parent);
}

void RigidBodyComponent::Update(float dt)
{
  // calculate physics
  m_RigidBody.integrate(dt);
}


RigidBodyComponent::Box::Box()
{
  body = new physics::RigidBody;
}

RigidBodyComponent::Box::~Box()
{
  delete body;
}

void RigidBodyComponent::Box::SetState(float mass, Vector3D position, Vector3D velocity, Vector3D rotation,
  Vector3D Acceleration, Vector3D halfSize_,
  float LinearDamping, float AngularDamping
)
{
  body->setPosition(position.x, position.y, position.z);
  body->setOrientation(1, 0, 0, 0);
  body->setVelocity(velocity.x, velocity.y, velocity.z);
  body->setRotation(rotation);

  halfSize = halfSize_;
  real mass_ = halfSize.x * halfSize.y * halfSize.z * mass;
  body->setMass(mass_);

  Matrix3x3 tensor;
  tensor.setBlockInertiaTensor(halfSize, mass_);
  body->setInertiaTensor(tensor);

  body->setLinearDamping(LinearDamping);
  body->setAngularDamping(AngularDamping);
  body->clearAccumulators();
  body->setAcceleration(Acceleration.x, Acceleration.y, Acceleration.z);

  body->setCanSleep(false);
  body->setAwake();

  body->calculateDerivedData();
  calculateInternals();
}

RigidBodyComponent::Sphere::Sphere()
{
  body = new physics::RigidBody;
}

RigidBodyComponent::Sphere::~Sphere()
{
  delete body;
}

void RigidBodyComponent::Sphere::setState(float mass, float radius_, Vector3D position, Vector3D velocity,
  Vector3D Acceleration,
  float LinearDamping, float AngularDamping)
{
  body->setMass(mass);
  body->setVelocity(velocity.x, velocity.y, velocity.z);
  body->setAcceleration(Acceleration.x, Acceleration.y, Acceleration.z);

  body->setDamping(LinearDamping, AngularDamping);
  radius = radius_;

  Matrix3x3 tensor;
  real coeff = 0.4f*body->getMass()*radius*radius;
  tensor.setInertiaTensorCoeffs(coeff, coeff, coeff);
  body->setInertiaTensor(tensor);

  // Set the data common to all particle types
  body->setPosition(position.x, position.y, position.z);
  // Clear the force accumulators
  body->calculateDerivedData();
  calculateInternals();
}

void RigidBodyComponent::Plane::setState(Vector3D direction_, real offset_)
{
  direction = direction_;
  offset = offset_;
}