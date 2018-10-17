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



	{
		TransformComponent* tcpSun = INFECT_GOM.GetGameObject(1)->GetComponent<TransformComponent>();	// 3
		if (INFECT_INPUT.IsKeyPressed(DIK_RIGHT)) {
			tcpSun->Move(Infect::GetFrameTime() *30.f*tcpSun->Right());
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_LEFT)) {
			tcpSun->Move(Infect::GetFrameTime() *-30.f*tcpSun->Right());
		}

		if (INFECT_INPUT.IsKeyPressed(DIK_UP)) {
			tcpSun->Move(Infect::GetFrameTime() *30.f * tcpSun->Forward());
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_DOWN)) {
			tcpSun->Move(Infect::GetFrameTime() *-30.f*tcpSun->Forward());
		}

		if (INFECT_INPUT.IsKeyPressed(DIK_I)) {
			tcpSun->Move(Infect::GetFrameTime() *30.f * tcpSun->Up());
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_K)) {
			tcpSun->Move(Infect::GetFrameTime() *-30.f*tcpSun->Up());
		}

		if (INFECT_INPUT.IsKeyPressed(DIK_J)) {
			tcpSun->RotateX(Infect::GetFrameTime() * 30.f);
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_L)) {
			tcpSun->RotateX(Infect::GetFrameTime() * -30.f);
		}

		if (INFECT_INPUT.IsKeyPressed(DIK_U)) {
			tcpSun->RotateY(Infect::GetFrameTime() * -30.f);
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_O)) {
			tcpSun->RotateY(Infect::GetFrameTime() * 30.f);
		}
	}

	if (INFECT_INPUT.IsKeyTriggered(DIK_P) || INFECT_INPUT.IsButtonTriggered(XBOX_BTN_BACK)) {
		INFECT_RENDERER.NextRenderMode();
	}
	{
		int xMove = -INFECT_INPUT.MousePosX() / 4;
		int yMove = -INFECT_INPUT.MousePosY() / 4;
		int xMoveGC = -INFECT_INPUT.GetRightAxisX() / 6500;
		int yMoveGC = -INFECT_INPUT.GetRightAxisY() / 6500;

		TransformComponent* tcpCamera = INFECT_GOM.GetGameObject(0)->GetComponent<TransformComponent>();
		if (xMove != 0)
			tcpCamera->RotateY(float(xMove));
		if (xMoveGC != 0)
			tcpCamera->RotateY(float(xMoveGC));
		if (yMove != 0)
			tcpCamera->RotateX(float(yMove));
		if (yMoveGC != 0)
			tcpCamera->RotateX(float(yMoveGC));

		float speed = 120.f;
		if (INFECT_INPUT.IsKeyPressed(DIK_D) || INFECT_INPUT.IsButtonPressed(XBOX_DPAD_RIGHT)) {
			tcpCamera->Move(Infect::GetFrameTime() *speed*tcpCamera->Right());
			//INFECT_AUDIOMANAGER.Set3dListener(Infect::GetFrameTime() *-speed *tcpCamera->Right(), Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0));
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_A) || INFECT_INPUT.IsButtonPressed(XBOX_DPAD_LEFT)) {
			tcpCamera->Move(Infect::GetFrameTime() *-speed * tcpCamera->Right());
			//INFECT_AUDIOMANAGER.Set3dListener(Infect::GetFrameTime() *speed*tcpCamera->Right(), Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0));

		}
		if (INFECT_INPUT.IsKeyPressed(DIK_W) || INFECT_INPUT.IsButtonPressed(XBOX_DPAD_UP)) {
			tcpCamera->Move(Infect::GetFrameTime() *speed * tcpCamera->Forward());
			//INFECT_AUDIOMANAGER.Set3dListener(Infect::GetFrameTime() *-speed * tcpCamera->Forward(), Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0));
		}
		if (INFECT_INPUT.IsKeyPressed(DIK_S) || INFECT_INPUT.IsButtonPressed(XBOX_DPAD_DOWN)) {
			tcpCamera->Move(Infect::GetFrameTime() *-speed * tcpCamera->Forward());
			//INFECT_AUDIOMANAGER.Set3dListener(Infect::GetFrameTime() *speed*tcpCamera->Forward(), Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0));
		}
	}

	{
		//if (INFECT_INPUT.IsKeyReleased(DIK_T) ) {
		//	INFECT_AUDIOMANAGER.TestingAudio();
		//}
		//if (INFECT_INPUT.IsKeyPressed(DIK_ESCAPE))
		//{
		//	INFECT_AUDIOMANAGER.PauseAudio(true);
		//}

		//Audio Testing
	}

	// alt+f4
	//if (INFECT_INPUT.IsKeyPressed(DIK_LALT) && INFECT_INPUT.IsKeyPressed(DIK_F4)) {
		//INFECT_GAME_STATE.SetGameState(GameState::QUIT);
		//INFECT_EVENTS.BroadcastEvent(&Event(EVENT_WINDOW_CLOSED));
	//}

#pragma endregion




	float deltaTime = INFECT_FRAMERATE.GetFrameTime();
	INFECT_INPUT.Update();							// Update input keys
													//INFECT_DEBUG.Update();						// Toggles debug drawing if needed
	INFECT_EVENTS.Update(deltaTime);				// Pump the event manager
	//Audio Update
	INFECT_AUDIOMANAGER.Update();

	//INFECT_GOM.Update(deltaTime);					// Update game logic
	INFECT_CMC.Update(deltaTime);



	//INFECT_GOM.UpdateStatus();					// Update status of game objects
	//INFECT_PHYSICS.Integrate(deltaTime);			// Move physics bodies
	//INFECT_PHYSICS.ResolveCollisions();			// Resolve collisions on physics bodies
	//INFECT_GOM.LateUpdate(deltaTime);				// Update game logic that occurs after physics
	INFECT_PHYSICS.Update(deltaTime);
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
