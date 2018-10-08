
#include <Stdafx.h>

RigidBodyComponent::RigidBodyComponent(InfectGUID guid) :
	Component(guid)
{
	
}

void RigidBodyComponent::Deactivate() {
	mp_Parent = nullptr;
}

int y = 0;
int x = 20;
void RigidBodyComponent::Serialize(const json& j)
{
	/*if (ValueExists(j, "Box"))
	{
		cur_type = RigidBodyType::BoxRigidBody;
		mp_newbox = new Box();
		//mp_newbox->SetState(5, Vector3D(21, 25, -10));
		mp_newbox->SetState(5, Vector3D(x, y, -10), Vector3D(0, 0, 0), Vector3D(0, 0, 0),
			Vector3D(0, 0, 0), Vector3D(2.5, 2.5, 2.5), 0.95f, 0.8f);
		INFECT_PHYSICS.m_BoxPool.push_back(mp_newbox);

		y += 25;
		x++;
		return;
	}*/
	

	m_position = Parent()->GetComponent<TransformComponent>()->WorldPosition();
	m_halfSize = Parent()->GetComponent<TransformComponent>()->GetScaleVector()*0.5;
	m_rotation = Parent()->GetComponent<TransformComponent>()->GetRotVector();
  // is box 
  if (ValueExists(j, "Box"))
  {
    Vector3D position_ = m_position, velocity_ = Vector3D(0, 0, 0),
      rotation_ = m_rotation, acceleration_ = Vector3D(0, 0, 0), halfSize_ = m_halfSize;
    float mass_ = 1., LinearDamping_ = 0.95f, AngularDamping_ = 0.8f;
    
	cur_type = RigidBodyType::BoxRigidBody;
      //position_.x = ParseFloat(j["position"], "x");
      //position_.y = ParseFloat(j["position"], "y");
      //position_.z = ParseFloat(j["position"], "z");
    

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

    //if (ValueExists(j, "scale")) {
    //  halfSize_.x = ValueExists(j["scale"], "x") ? j["scale"]["x"] : halfSize_.x;
    //  halfSize_.y = ValueExists(j["scale"], "y") ? j["scale"]["y"] : halfSize_.y;
    //  halfSize_.z = ValueExists(j["scale"], "z") ? j["scale"]["z"] : halfSize_.z;
    //}

    if (ValueExists(j, "linearDamping")) {
      LinearDamping_ = ParseFloat(j, "linearDamping");
    }

    if (ValueExists(j, "angularDamping")) {
      AngularDamping_ = ParseFloat(j, "angularDamping");
    }

	mp_newbox = new Box();
	mp_newbox->SetState(mass_, position_, velocity_, rotation_, acceleration_, halfSize_, LinearDamping_, AngularDamping_);
    INFECT_PHYSICS.m_BoxPool.push_back(mp_newbox);
  }

  else if (ValueExists(j, "Sphere"))
  {
    Vector3D position_ = m_position, velocity_ = Vector3D(0, 0, 0),
       acceleration_ = Vector3D(0, 0, 0);
    float mass_ = 1., LinearDamping_ = 0.95f, AngularDamping_ = 0.8f, radius_ = 0.5f;

	cur_type = RigidBodyType::SphereRigidBody;

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

	mp_newSphere = new Sphere();
	mp_newSphere->setState(mass_, radius_, position_, velocity_, acceleration_, LinearDamping_, AngularDamping_);
    INFECT_PHYSICS.m_SpherePool.push_back(mp_newSphere);
  }

  else if (ValueExists(j, "Plane"))
  {
    Vector3D direction_ = Vector3D(0, 1, 0);
    float offset_ = 0.;

	cur_type = RigidBodyType::PlaneRigidBody;

    if (ValueExists(j, "direction")) {
      direction_.x = ValueExists(j["direction"], "x") ? j["direction"]["x"] : direction_.x;
      direction_.y = ValueExists(j["direction"], "y") ? j["direction"]["y"] : direction_.y;
      direction_.z = ValueExists(j["direction"], "z") ? j["direction"]["z"] : direction_.z;
    }

    if (ValueExists(j, "offset")) {
      offset_ = ParseFloat(j, "offset");
    }

	/*direction_ = Vector3D(0.f, 0.f, -1.f, 0.f);
	auto aa = Parent()->GetComponent<TransformComponent>()->GetRotVector();
	auto RotationMatrix = Parent()->GetComponent<TransformComponent>()->GetRotationMatrix();
	direction_ = RotationMatrix * direction_;*/
	offset_ = m_position.y;
	

	mp_newPlane = new Plane();
	mp_newPlane->setState(direction_, offset_);
    INFECT_PHYSICS.m_PlanePool.push_back(mp_newPlane);
  }

}


void RigidBodyComponent::Override(const json& j)
{
	//m_position = Parent()->GetComponent<TransformComponent>()->WorldPosition();
	//m_halfSize = Parent()->GetComponent<TransformComponent>()->GetScaleVector()*0.5;
	//m_rotation = Parent()->GetComponent<TransformComponent>()->GetRotVector();
	// is box 
	if (ValueExists(j, "Box"))
	{
		Vector3D position_ = mp_newbox->body->getPosition(), velocity_ = mp_newbox->body->getVelocity(),
			rotation_ = mp_newbox->body->getRotation(),
			acceleration_ = mp_newbox->body->getAcceleration(),
			halfSize_ = mp_newbox->halfSize;
		float mass_ = mp_newbox->body->getMass(), LinearDamping_ = mp_newbox->body->getLinearDamping(),
			AngularDamping_ = mp_newbox->body->getAngularDamping(),

		cur_type = RigidBodyType::BoxRigidBody;
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
			acceleration_.x = ValueExists(j["acceleration"], "x") ? j["acceleration"]["x"] : acceleration_.x;
			acceleration_.y = ValueExists(j["acceleration"], "y") ? j["acceleration"]["y"] : acceleration_.y;
			acceleration_.z = ValueExists(j["acceleration"], "z") ? j["acceleration"]["z"] : acceleration_.z;
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

		
		mp_newbox->SetState(mass_, position_, velocity_, rotation_, acceleration_, halfSize_, LinearDamping_, AngularDamping_);
		//INFECT_PHYSICS.m_BoxPool.push_back(newbox);
	}

	else if (ValueExists(j, "Sphere"))
	{
		Vector3D position_ = mp_newSphere->body->getPosition(), velocity_ = mp_newSphere->body->getVelocity(),
			acceleration_ = mp_newSphere->body->getAcceleration();
		float mass_ = mp_newSphere->body->getMass(), LinearDamping_ = mp_newSphere->body->getLinearDamping(), 
			AngularDamping_ = mp_newSphere->body->getAngularDamping(),
			radius_ = mp_newSphere->radius;

		cur_type = RigidBodyType::SphereRigidBody;

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
			acceleration_.x = ValueExists(j["acceleration"], "x") ? j["acceleration"]["x"] : acceleration_.x;
			acceleration_.y = ValueExists(j["acceleration"], "y") ? j["acceleration"]["y"] : acceleration_.y;
			acceleration_.z = ValueExists(j["acceleration"], "z") ? j["acceleration"]["z"] : acceleration_.z;
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

		mp_newSphere->setState(mass_, radius_, position_, velocity_, acceleration_, LinearDamping_, AngularDamping_);
	}

	else if (ValueExists(j, "Plane"))
	{
		Vector3D direction_ = mp_newPlane->direction;
		float offset_ = mp_newPlane->offset;

		cur_type = RigidBodyType::PlaneRigidBody;

		if (ValueExists(j, "direction")) {
			direction_.x = ValueExists(j["direction"], "x") ? j["direction"]["x"] : direction_.x;
			direction_.y = ValueExists(j["direction"], "y") ? j["direction"]["y"] : direction_.y;
			direction_.z = ValueExists(j["direction"], "z") ? j["direction"]["z"] : direction_.z;
		}

		if (ValueExists(j, "offset")) {
			offset_ = ParseFloat(j, "offset");
		}

		mp_newPlane->setState(direction_, offset_);
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
  Quaternion temp;
  //body->setOrientation(temp.DegreeToQuaternion(Vector3D(45, 45, 45)));

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
  body->setAcceleration(Vector3D::GRAVITY + Acceleration);
  //body->setAcceleration(Acceleration);
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
  body->setAcceleration(Vector3D::GRAVITY + Acceleration);

  body->setDamping(LinearDamping, AngularDamping);
  body->clearAccumulators();
  radius = radius_;

  Matrix3x3 tensor;
  real coeff = 0.4f*body->getMass()*radius*radius;
  tensor.setInertiaTensorCoeffs(coeff, coeff, coeff);
  body->setInertiaTensor(tensor);



  body->setCanSleep(false);
  body->setAwake();
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
