#include "Stdafx.h"

void SimulationThreadContainer::RunThread()
{
	while (!m_Terminate) {
		if (!mp_CurrentCommand) {
			if (INFECT_THREAD_JOBS.HasJobs(Type)) {
				mp_CurrentCommand = INFECT_THREAD_JOBS.GetThreadJob(Type);
				//std::cout << "Simulation Thread : Work received." << std::endl;
			}
			//std::cout << "Simulation Thread : Idling..." << std::endl;
		}

		if (mp_CurrentCommand) {
			if (mp_CurrentCommand->execute()) {
				//std::cout << "Simulation Thread : Work completed." << std::endl;
				INFECT_THREAD_JOBS.FinishedJob(Type);
				mp_CurrentCommand = nullptr;
			}
			//std::cout << "Simulation Thread : Working..." << std::endl;
		}
	}
	std::cout << "Simulation Thread : Terminating." << std::endl;
}

std::thread & SimulationThreadContainer::Spawn()
{
	mp_Thread = new std::thread(&SimulationThreadContainer::RunThread, this);
	return *mp_Thread;
}

bool StartSimulationCommand::execute() const
{
	INFECT_FRAMERATE.FrameStart();


#pragma region TestStuff
	TransformComponent* tcpCamera = INFECT_GOM.GetGameObject(2)->GetComponent<TransformComponent>();  // 2
	TransformComponent* tcpLight = INFECT_GOM.GetGameObject(3)->GetComponent<TransformComponent>();	// 3
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

	if (INFECT_INPUT.IsButtonPressed(XBOX_DPAD_RIGHT) || INFECT_INPUT.IsKeyPressed(DIK_D)) {
		tcpCamera->Move(Infect::GetFrameTime() *30.f*tcpCamera->Right());
	}
	if (INFECT_INPUT.IsButtonPressed(XBOX_DPAD_LEFT) || INFECT_INPUT.IsKeyPressed(DIK_A)) {
		tcpCamera->Move(Infect::GetFrameTime() *-30.f*tcpCamera->Right());
	}
	if (INFECT_INPUT.IsButtonPressed(XBOX_DPAD_UP) || INFECT_INPUT.IsKeyPressed(DIK_W)) {
		tcpCamera->Move(Infect::GetFrameTime() *30.f * tcpCamera->Forward());
	}
	if (INFECT_INPUT.IsButtonPressed(XBOX_DPAD_DOWN) || INFECT_INPUT.IsKeyPressed(DIK_S)) {
		tcpCamera->Move(Infect::GetFrameTime() *-30.f*tcpCamera->Forward());
	}
	if (INFECT_INPUT.IsKeyPressed(DIK_SPACE)) {
		tcpCamera->SetPosition(Vector3D(0, 0, 50, 1));
	}
	// alt+f4
	if (INFECT_INPUT.IsKeyPressed(DIK_LALT) && INFECT_INPUT.IsKeyPressed(DIK_F4)) {
		INFECT_GAME_STATE.SetGameState(GameState::QUIT);
	}
	INFECT_GOM.GetGameObject(0)->GetComponent<TransformComponent>()->SetPosition(tcpCamera->WorldPosition());
	//pGO->GetComponent<TransformComponent>()->RotateY(Infect::GetFrameTime() * 50.0f);
	//pGO->GetComponent<TransformComponent>()->RotateZ(Infect::GetFrameTime() * 25.0f);

#pragma endregion




	float deltaTime = INFECT_FRAMERATE.GetFrameTime();
	INFECT_INPUT.Update();							// Update input keys
													//INFECT_DEBUG.Update();						// Toggles debug drawing if needed
	INFECT_EVENTS.Update(deltaTime);				// Pump the event manager
													//INFECT_AUDIO.Update(deltaTime);

	//INFECT_GOM.Update(deltaTime);					// Update game logic
	INFECT_CMC.Update(deltaTime);



	//INFECT_GOM.UpdateStatus();					// Update status of game objects
	//INFECT_PHYSICS.Integrate(deltaTime);			// Move physics bodies
	//INFECT_PHYSICS.ResolveCollisions();			// Resolve collisions on physics bodies
	//INFECT_GOM.LateUpdate(deltaTime);				// Update game logic that occurs after physics

	INFECT_THREAD_JOBS.AddNewJob(new StartRenderCommand(*INFECT_THREAD_JOBS.GetThreadContainer<RenderThreadContainer>(ThreadType::RenderThread)));
	//INFECT_IMGUI.Update();						// Update all Imgui commands
	return true;
}

bool SimulationTerminateTerminate::execute() const
{
	if (tCont.IsBusy())
		return false;
	tCont.Terminate();
	return true;
}
