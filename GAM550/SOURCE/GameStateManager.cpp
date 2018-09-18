/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

GameStateManager::GameStateManager() :
	m_currentState(GameState::CURRENT_LEVEL),
	m_nextState(GameState::CURRENT_LEVEL),
	m_debugPause(false), m_isGamePaused(false), m_isLevelOver(false)
{

}

GameStateManager::~GameStateManager() {}

MSG GameStateManager::Update() {
	// this struct holds Windows event messages
	MSG msg;








	SeedRand(0);

	GameObject* pGOSkyBox = INFECT_GOM.SpawnGameObject();
	MeshComponent* pMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
	pMeshComp->SetScene(SKYBOX_PRIMITIVE);
	pMeshComp->SetDiffuseTexture("worldTexture2.jpg");
	pMeshComp->SetIsLit(false);

	TransformComponent* pTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
	pTransComp->SetPosition(Vector3D(0, 0, 0, 1));
	//pTransComp->SetAngleX(-90);
	//pTransComp->SetAngleZ(45);
	//pTransComp->SetScale(500.0f, 500.0f, 500.0f);
	pTransComp->SetScale(500.0f, 500.0f, 500.0f);

	pGOSkyBox->AddComponent(pMeshComp);
	pGOSkyBox->AddComponent(pTransComp);
	pGOSkyBox->LateInitialize();


	GameObject* pGO = INFECT_GOM.SpawnGameObject();
	pMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
	//pMeshComp->SetScene("Suzy.fbx");
	//pMeshComp->SetScene(PLANE_PRIMITIVE);
	//pMeshComp->SetScene(CUBE_PRIMITIVE);
	//pMeshComp->SetScene(SKYBOX_PRIMITIVE);
	pMeshComp->SetScene(SPHERE_PRIMITIVE);
	//pMeshComp->SetScene(POLAR_SPHERE_PRIMITIVE);
	//pMeshComp->SetScene("test");
	//pMeshComp->SetDiffuseTexture("worldTexture2.jpg");

	pTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
	pTransComp->SetPosition(Vector3D(0, 0, 0, 1));
	//pTransComp->SetAngleX(-90);
	//pTransComp->SetAngleZ(45);
	//pTransComp->SetScale(500.0f, 500.0f, 500.0f);
	pTransComp->SetScale(10.0f, 10.0f, 10.0f);

	pGO->AddComponent(pMeshComp);
	pGO->AddComponent(pTransComp);
	pGO->LateInitialize();

	GameObject* pGOCamera = INFECT_GOM.SpawnGameObject();
	TransformComponent* pTransComp2 = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
	pTransComp2->SetPosition(Vector3D(0, 0, 50, 1));

	CameraComponent * pCamComp = (CameraComponent*)INFECT_MEMORY.GetNewComponent(CameraComponent::Type);
	pGOCamera->AddComponent(pTransComp2);
	pGOCamera->AddComponent(pCamComp);
	pGOCamera->LateInitialize();

	GameObject* pGOLight = INFECT_GOM.SpawnGameObject();
	PointLightComponent *pLight = (PointLightComponent*)INFECT_MEMORY.GetNewComponent(PointLightComponent::Type);
	TransformComponent* pTransComp3 = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
	pTransComp3->SetPosition(Vector3D(0, 10, 10, 1));
	pGOLight->AddComponent(pLight);
	pGOLight->AddComponent(pTransComp3);
	pGOLight->LateInitialize();






	TransformComponentManager* tcm = static_cast<TransformComponentManager*>(INFECT_CMC.GetCM(0));
	tcm->Register(pGOSkyBox->GetComponent<TransformComponent>());
	tcm->Register(pGO->GetComponent<TransformComponent>());
	tcm->Register(pGOCamera->GetComponent<TransformComponent>());
	tcm->Register(pGOLight->GetComponent<TransformComponent>());
	CameraComponentManager* ccm = static_cast<CameraComponentManager*>(INFECT_CMC.GetCM(1));
	ccm->Register(pGOCamera->GetComponent<CameraComponent>());
	PointLightComponentManager* plcm = static_cast<PointLightComponentManager*>(INFECT_CMC.GetCM(2));
	plcm->Register(pGOLight->GetComponent<PointLightComponent>());
	MeshComponentManager* mcm = static_cast<MeshComponentManager*>(INFECT_CMC.GetCM(3));
	mcm->Register(pGOSkyBox->GetComponent<MeshComponent>());
	mcm->Register(pGO->GetComponent<MeshComponent>());





	GameObject* pGOFOR[8];
	Vector3D pos[8] = {
		Vector3D(25, 25, 25, 1),
		Vector3D(-25, 25, 25, 1),
		Vector3D(25, -25, 25, 1),
		Vector3D(-25, -25, 25, 1),

		Vector3D(25, 25, -25, 1),
		Vector3D(-25, 25, -25, 1),
		Vector3D(25, -25, -25, 1),
		Vector3D(-25, -25, -25, 1)
	};
	for (int i = 0; i<8; ++i)
	{
		pGOFOR[i] = INFECT_GOM.SpawnGameObject();
		pMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
		pMeshComp->SetScene(CUBE_PRIMITIVE);
		pMeshComp->SetIsLit(false);

		pTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
		pTransComp->SetPosition(pos[i]);
		pTransComp->SetScale(1.0f, 1.0f, 1.0f);

		pGOFOR[i]->AddComponent(pMeshComp);
		pGOFOR[i]->AddComponent(pTransComp);
		pGOFOR[i]->LateInitialize();

		tcm->Register(pGOFOR[i]->GetComponent<TransformComponent>());
		mcm->Register(pGOFOR[i]->GetComponent<MeshComponent>());
	}


	while (m_currentState != GameState::QUIT) {

		m_currentState = GameState::CURRENT_LEVEL;
		m_nextState = GameState::CURRENT_LEVEL;
		
		// Game loop
		// wait for the next message in the queue, store the result in 'msg'
		while (m_currentState == m_nextState) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				// translate keystroke messages into the right format
				TranslateMessage(&msg);
				// send the message to the WindowProc function
				DispatchMessage(&msg);
			}

			Infect::FrameStart();

			TransformComponent* tcpCamera = pGOCamera->GetComponent<TransformComponent>();
			TransformComponent* tcpLight = pGOLight->GetComponent<TransformComponent>();
			if (INFECT_INPUT.IsKeyPressed(DIK_RIGHT) || INFECT_INPUT.IsKeyPressed(DIK_RIGHT)) {
				tcpLight->Move(Infect::GetFrameTime() *30.f*tcpLight->Right());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_LEFT)) {
				tcpLight->Move(Infect::GetFrameTime() *-30.f*tcpLight->Right());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_UP)) {
				tcpLight->Move(Infect::GetFrameTime() *30.f * tcpLight->Up());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_DOWN)) {
				tcpLight->Move(Infect::GetFrameTime() *-30.f*tcpLight->Up());
			}


			int xMove = -INFECT_INPUT.MousePosX() / 4;
			int yMove = -INFECT_INPUT.MousePosY() / 4;
			if (xMove != 0)
				tcpCamera->RotateY(float(xMove));
			if (yMove != 0)
				tcpCamera->RotateX(float(yMove));

			if (INFECT_INPUT.IsKeyPressed(DIK_D)) {
				tcpCamera->Move(Infect::GetFrameTime() *30.f*tcpCamera->Right());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_A)) {
				tcpCamera->Move(Infect::GetFrameTime() *-30.f*tcpCamera->Right());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_W)) {
				tcpCamera->Move(Infect::GetFrameTime() *30.f * tcpCamera->Forward());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_S)) {
				tcpCamera->Move(Infect::GetFrameTime() *-30.f*tcpCamera->Forward());
			}
			if (INFECT_INPUT.IsKeyPressed(DIK_SPACE)) {
				tcpCamera->SetPosition(Vector3D(0, 0, 50, 1));
			}
			// alt+f4
			if (INFECT_INPUT.IsKeyPressed(DIK_LALT) && INFECT_INPUT.IsKeyPressed(DIK_F4)) {
				INFECT_GAME_STATE.SetGameState(GameState::QUIT);
			}
			pGOSkyBox->GetComponent<TransformComponent>()->SetPosition(tcpCamera->WorldPosition());
			pGO->GetComponent<TransformComponent>()->RotateY(Infect::GetFrameTime() * 50.0f);
			//pGO->GetComponent<TransformComponent>()->RotateZ(Infect::GetFrameTime() * 25.0f);
			Infect::Update(Infect::GetFrameTime());			// Game loop

			Infect::FrameEnd();
		}

		m_currentState = m_nextState;
	}

	Infect::UnloadResources();	// Unloads all resources
	Infect::Cleanup();

	return msg;
}

void GameStateManager::HandleEvent(Event * p_event) {
	switch (p_event->Type()) {
		case EVENT_WINDOW_CLOSED: {
			SetGameState(GameState::QUIT);
			break;
		}
		case RESTART_LEVEL: {
			SetGameState(GameState::RESTART);
			break;
		}
		case EVENT_INPUT_PAUSEGAME: {
			if (m_isLevelOver) return;
			InputButtonData* pData = p_event->Data<InputButtonData>();
			if (pData->m_isTrigger) {
				PauseGame(!m_isGamePaused);
				if(m_isGamePaused) INFECT_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnPauseGame));
				else INFECT_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnGameResume));
			}
			break;
		}
		case EVENT_LevelInComplete: {
			m_isLevelOver = true;
			PauseGame(true);
			break;
		}
		case EVENT_ExitLevel: {
			m_isLevelOver = true;
			break;
		}
	}
}

void GameStateManager::SetGameState(GameState state) { 
	m_nextState = state; 
	PauseGame(false);
	m_isLevelOver = false;
}