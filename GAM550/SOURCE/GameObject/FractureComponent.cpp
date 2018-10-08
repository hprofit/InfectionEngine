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

}

void FractureComponent::HandleEvent(Event * pEvent){}

void FractureComponent::LateInitialize(){}