/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ControllerComponentManager::ControllerComponentManager() {
	m_Components = (std::list<ControllerComponent*>*)INFECT_MEMORY.GetComponentPool(ControllerComponent::Type);
}

void ControllerComponentManager::Update(float dt) {
	for each (CC bComp in *m_Components) {
		if (!bComp->IsActive()) break;

		// HARD-CODED FOR ENGINE DEMO
		std::string str = "Bullet";
		if (INFECT_INPUT.IsKeyTriggered(DIK_SPACE)) {
			GameObject* pCam = INFECT_GOM.GetCamera();
			GameObject* pGO = INFECT_GOM.SpawnGameObject(str);

			Vector3D direction = pCam->GetComponent<TransformComponent>()->LookAt();
			Vector3D pos = pCam->GetComponent<TransformComponent>()->WorldPosition();

			pGO->GetComponent<TransformComponent>()->SetPosition(pos);
			pGO->GetComponent<RigidBodyComponent>()->mp_newbox->body->setPosition(pos);
			pGO->GetComponent<RigidBodyComponent>()->mp_newbox->body->setVelocity(direction*100.0f);
		}
	}
}

// TODO: Super naive attempt at this
void ControllerComponentManager::HandleEvent(Event * pEvent) {
	for each (CC tComp in *m_Components) {
		if (!tComp->IsActive()) break;
		tComp->HandleEvent(pEvent);
	}
}

void ControllerComponentManager::Init() {
	INFECT_MEMORY.ComponentPoolInit<ControllerComponent>(ControllerComponent::Type, ControllerComponent::CACHESIZE);
}