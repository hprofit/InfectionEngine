 /* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

Attack::Attack(BehaviorTree* tree){
	SetStatus(BH_INVALID);
	m_attackDelay = 0.5;
}

void Attack::OnInitialize() {
	m_sinceOnInit = 0.0f;
}

BH_Status Attack::OnUpdate() {
	m_sinceOnInit += INFECT_FRAMERATE.GetFrameTime();
	
	std::string str = "Bullet";
	if (m_attackDelay < m_sinceOnInit) {
		GameObject* pCam = pAgent->Parent();
		GameObject* pGO = INFECT_GOM.SpawnGameObject(str);

		Vector3D direction = pCam->GetComponent<TransformComponent>()->LookAt();
		Vector3D pos = pCam->GetComponent<TransformComponent>()->WorldPosition();

		pGO->GetComponent<TransformComponent>()->SetPosition(pos);
		pGO->GetComponent<RigidBodyComponent>()->mp_newbox->body->setPosition(pos);
		pGO->GetComponent<RigidBodyComponent>()->mp_newbox->body->setVelocity(direction*100.0f);
		return BH_SUCCESS;
	}
	return BH_RUNNING;
}

void Attack::OnTerminate(BH_Status) {
	SetStatus(BH_INVALID);
}


void Attack::Serialize(const json& j) {

}