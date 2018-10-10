#include <Stdafx.h>

FractureComponent::FractureComponent(InfectGUID guid) :
  Component(guid)
{
}

void FractureComponent::Deactivate() {
  mp_Parent = nullptr;
}

void FractureComponent::Serialize(const json& j)
{
  //mp_Target = Parent()->GetComponent<RigidBodyComponent>()->GetBoxPointer();

  if (ValueExists(j, "Box"))
  {
    mp_Target = Parent()->GetComponent<RigidBodyComponent>()->GetBoxPointer();

    if (ValueExists(j, "CanBeBroken"))
      mp_Target->m_CanBeBroken = true;

    if (ValueExists(j, "Breaker"))
      mp_Target->m_Breaker = true;
  }

  if (ValueExists(j, "Sphere"))
  {
    if (ValueExists(j, "Breaker"))
      Parent()->GetComponent<RigidBodyComponent>()->GetSpherePointer()->m_Breaker = true;
  }
}

void FractureComponent::Override(const json& j)
{
  Serialize(j);
}

void FractureComponent::HandleEvent(Event * pEvent){}

void FractureComponent::LateInitialize(){}