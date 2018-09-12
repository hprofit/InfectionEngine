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










	Mesh* pMesh = new Mesh();

	pMesh->AddVertex(-1.0f, 1.0f, -1.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pMesh->AddVertex(1.0f, 1.0f, -1.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pMesh->AddVertex(-1.0f, -1.0f, -1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pMesh->AddVertex(1.0f, -1.0f, -1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pMesh->AddVertex(-1.0f, 1.0f, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	pMesh->AddVertex(1.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pMesh->AddVertex(-1.0f, -1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pMesh->AddVertex(1.0f, -1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	pMesh->AddFace(0, 1, 2);
	pMesh->AddFace(2, 1, 3);
	pMesh->AddFace(4, 0, 6);
	pMesh->AddFace(6, 0, 2);
	pMesh->AddFace(7, 5, 6);
	pMesh->AddFace(6, 5, 4);
	pMesh->AddFace(3, 1, 7);
	pMesh->AddFace(7, 1, 5);
	pMesh->AddFace(4, 5, 0);
	pMesh->AddFace(0, 5, 1);
	pMesh->AddFace(3, 7, 2);
	pMesh->AddFace(2, 7, 6);

	pMesh->FinishMesh();
	Scene* pScene = new Scene(1);
	(*pScene)[0] = pMesh;
	
	GameObject* pGO = INFECT_GOM.SpawnGameObject();
	MeshComponent* pMeshComp = new MeshComponent();
	pMeshComp->SetScene(INFECT_RESOURCES.GetScene("Suzy.fbx"));
	//pMeshComp->SetScene(pScene);
	
	TransformComponent* pTransComp = new TransformComponent();
	pTransComp->SetPosition(Vector3D(0, 0, 0, 1));
	pTransComp->SetAngleX(-90);
	//pTransComp->SetAngleZ(45);
	pTransComp->SetScale(10.0f, 10.0f, 10.0f);
	
	pGO->AddComponent(pMeshComp);
	pGO->AddComponent(pTransComp);
	pGO->LateInitialize();
	
	pGO->Update(0);
	pGO->LateUpdate(0);
	
	GameObject* pGOCamera = INFECT_GOM.SpawnGameObject();
	TransformComponent* pTransComp2 = new TransformComponent();
	pTransComp2->SetPosition(Vector3D(0, 0, 50, 1));
	
	CameraComponent * pCamComp = new CameraComponent();
	pGOCamera->AddComponent(pTransComp2);
	pGOCamera->AddComponent(pCamComp);
	pGOCamera->LateInitialize();
	
	pGOCamera->Update(0);
	pGOCamera->LateUpdate(0);















	while (m_currentState != GameState::QUIT) {

		m_currentState = GameState::CURRENT_LEVEL;
		m_nextState = GameState::CURRENT_LEVEL;
		
		// Game loop
		// wait for the next message in the queue, store the result in 'msg'
		while (m_currentState == m_nextState) {
			//BOOL bRet;
			GetMessage(&msg, NULL, 0, 0);
			//while (bRet = GetMessage(&msg, NULL, 0, 0) != 0) {
			//	if (bRet == -1) {
			//		// handle errors
			//	} 
			//	else {
			//		// translate keystroke messages into the right format
			//		TranslateMessage(&msg);
			//		// send the message to the WindowProc function
			//		DispatchMessage(&msg);


					Infect::FrameStart();
					pTransComp->RotateZ(Infect::GetFrameTime() * 100.0f);
					if (INFECT_INPUT.IsKeyPressed(DIK_RIGHT)) {
						pTransComp2->Move(0.3f*pTransComp2->Right());
					}
					if (INFECT_INPUT.IsKeyPressed(DIK_LEFT)) {
						pTransComp2->Move(-0.3f*pTransComp2->Right());
					}
					if (INFECT_INPUT.IsKeyPressed(DIK_UP)) {
						pTransComp2->Move(0.3f * pTransComp2->Up());
					}
					if (INFECT_INPUT.IsKeyPressed(DIK_DOWN)) {
						pTransComp2->Move(-0.3f*pTransComp2->Up());
					}
					if (INFECT_INPUT.IsKeyPressed(DIK_SPACE)) {
						pTransComp2->SetPosition(Vector3D(0, 0, 50, 1));
					}
					pTransComp->RotateZ(Infect::GetFrameTime() * 50.0f);
					Infect::Update(Infect::GetFrameTime());			// Game loop

					Infect::FrameEnd();
				//}
			//}
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