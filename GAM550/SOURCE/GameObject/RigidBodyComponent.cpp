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
	//INFECT_GOM.RegisterCamera(mp_Parent);
}