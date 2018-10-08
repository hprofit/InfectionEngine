/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ControllerComponent::ControllerComponent(InfectGUID guid) :
	Component(guid) {
}

void ControllerComponent::Deactivate() {
	mp_Parent = nullptr;
}

void ControllerComponent::LateInitialize() {
}

void ControllerComponent::Serialize(const json& j) {

}

void ControllerComponent::Override(const json& j) {

}

void ControllerComponent::HandleEvent(Event * p_event) {

}
