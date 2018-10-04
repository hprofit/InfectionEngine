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
  // is box 
  if (ValueExists(j, "Box"))
  {
    Vector3D position_ = Vector3D(0, 0, 0), velocity_ = Vector3D(0, 0, 0),
      rotation_ = Vector3D(0, 0, 0), acceleration_ = Vector3D(0, 0, 0), halfSize_ = Vector3D(1, 1, 1);
    float mass_ = 1., LinearDamping_ = 0.95f, AngularDamping_ = 0.8f;
    if (ValueExists(j, "position")) {
      position_.x = ValueExists(j["position"], "x") ? j["position"]["x"] : position_.x;
      position_.y = ValueExists(j["position"], "y") ? j["position"]["y"] : position_.y;
      position_.z = ValueExists(j["position"], "z") ? j["position"]["z"] : position_.z;
    }

    if (ValueExists(j, "mass")) {
      mass_ = ParseFloat(j, "mass");
    }

    if (ValueExists(j, "velocity")) {
      velocity_.x = ValueExists(j["velocity"], "x") ? j["velocity"]["x"] : velocity_.x;
      velocity_.y = ValueExists(j["velocity"], "y") ? j["velocity"]["y"] : velocity_.y;
      velocity_.z = ValueExists(j["velocity"], "z") ? j["velocity"]["z"] : velocity_.z;
    }

    if (ValueExists(j, "rotation")) {
      rotation_.x = ValueExists(j["rotation"], "x") ? j["rotation"]["x"] : rotation_.x;
      rotation_.y = ValueExists(j["rotation"], "y") ? j["rotation"]["y"] : rotation_.y;
      rotation_.z = ValueExists(j["rotation"], "z") ? j["rotation"]["z"] : rotation_.z;
    }

    if (ValueExists(j, "acceleration")) {
      acceleration_.x = ValueExists(j["rotation"], "x") ? j["rotation"]["x"] : acceleration_.x;
      acceleration_.y = ValueExists(j["rotation"], "y") ? j["rotation"]["y"] : acceleration_.y;
      acceleration_.z = ValueExists(j["rotation"], "z") ? j["rotation"]["z"] : acceleration_.z;
    }

    if (ValueExists(j, "scale")) {
      halfSize_.x = ValueExists(j["scale"], "x") ? j["scale"]["x"] : halfSize_.x;
      halfSize_.y = ValueExists(j["scale"], "y") ? j["scale"]["y"] : halfSize_.y;
      halfSize_.z = ValueExists(j["scale"], "z") ? j["scale"]["z"] : halfSize_.z;
    }

    if (ValueExists(j, "linearDamping")) {
      LinearDamping_ = ParseFloat(j, "linearDamping");
    }

    if (ValueExists(j, "angularDamping")) {
      AngularDamping_ = ParseFloat(j, "angularDamping");
    }

    Box *newbox = new Box();
    newbox->SetState(mass_, position_, velocity_, rotation_, acceleration_, halfSize_, LinearDamping_, AngularDamping_);
    INFECT_PHYSICS.m_BoxPool.push_back(newbox);
  }

  else if (ValueExists(j, "Sphere"))
  {
    Vector3D position_ = Vector3D(0, 0, 0), velocity_ = Vector3D(0, 0, 0),
       acceleration_ = Vector3D(0, 0, 0);
    float mass_ = 1., LinearDamping_ = 0.95f, AngularDamping_ = 0.8f, radius_ = 0.5f;
    if (ValueExists(j, "position")) {
      position_.x = ValueExists(j["position"], "x") ? j["position"]["x"] : position_.x;
      position_.y = ValueExists(j["position"], "y") ? j["position"]["y"] : position_.y;
      position_.z = ValueExists(j["position"], "z") ? j["position"]["z"] : position_.z;
    }

    if (ValueExists(j, "mass")) {
      mass_ = ParseFloat(j, "mass");
    }

    if (ValueExists(j, "velocity")) {
      velocity_.x = ValueExists(j["velocity"], "x") ? j["velocity"]["x"] : velocity_.x;
      velocity_.y = ValueExists(j["velocity"], "y") ? j["velocity"]["y"] : velocity_.y;
      velocity_.z = ValueExists(j["velocity"], "z") ? j["velocity"]["z"] : velocity_.z;
    }   

    if (ValueExists(j, "acceleration")) {
      acceleration_.x = ValueExists(j["rotation"], "x") ? j["rotation"]["x"] : acceleration_.x;
      acceleration_.y = ValueExists(j["rotation"], "y") ? j["rotation"]["y"] : acceleration_.y;
      acceleration_.z = ValueExists(j["rotation"], "z") ? j["rotation"]["z"] : acceleration_.z;
    }

    if (ValueExists(j, "radius")) {
      radius_ = ParseFloat(j, "radius");
    }

    if (ValueExists(j, "linearDamping")) {
      LinearDamping_ = ParseFloat(j, "linearDamping");
    }

    if (ValueExists(j, "angularDamping")) {
      AngularDamping_ = ParseFloat(j, "angularDamping");
    }

    Sphere *newSphere = new Sphere();
    newSphere->setState(mass_, radius_, position_, velocity_, acceleration_, LinearDamping_, AngularDamping_);
    INFECT_PHYSICS.m_SpherePool.push_back(newSphere);
  }

  else if (ValueExists(j, "Plane"))
  {
    Vector3D direction_ = Vector3D(0, 1, 0);
    float offset_ = 0.;
    if (ValueExists(j, "direction")) {
      direction_.x = ValueExists(j["direction"], "x") ? j["direction"]["x"] : direction_.x;
      direction_.y = ValueExists(j["direction"], "y") ? j["direction"]["y"] : direction_.y;
      direction_.z = ValueExists(j["direction"], "z") ? j["direction"]["z"] : direction_.z;
    }

    if (ValueExists(j, "offset")) {
      offset_ = ParseFloat(j, "offset");
    }

    Plane *newPlane = new Plane();
    newPlane->setState(direction_, offset_);
    INFECT_PHYSICS.m_PlanePool.push_back(newPlane);
  }

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
  //m_RigidBody.integrate(dt);
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

void RigidBodyComponent::Box::calculateMassProperties(real invDensity)
{
  // Check for infinite mass
  if (invDensity <= 0)
  {
    // Just set zeros for both mass and inertia tensor
    body->setInverseMass(0);
    body->setInverseInertiaTensor(Matrix3x3(0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f));
  }
  else
  {
    // Otherwise we need to calculate the mass
    real volume = halfSize.Length() * 2.0;
    real mass = volume / invDensity;

    body->setMass(mass);

    // And calculate the inertia tensor from the mass and size
    mass *= 0.333f;
    Matrix3x3 tensor;
    tensor.setInertiaTensorCoeffs(
      mass * halfSize.y*halfSize.y + halfSize.z*halfSize.z,
      mass * halfSize.y*halfSize.x + halfSize.z*halfSize.z,
      mass * halfSize.y*halfSize.x + halfSize.z*halfSize.y
    );
    body->setInertiaTensor(tensor);
  }

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