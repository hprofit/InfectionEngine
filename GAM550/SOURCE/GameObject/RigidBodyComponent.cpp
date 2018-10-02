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