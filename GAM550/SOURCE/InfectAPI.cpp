/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

namespace Infect {

	int Initialize(std::string configFile, HINSTANCE hInstance, int nCmdShow)
	{
		INFECT_EVENTS.Init();
		INFECT_THREAD_JOBS.Init();
		INFECT_GAME_CONFIG.LoadConfig(configFile);
		INFECT_GOM.Init();
		if (INFECT_GAME_CONFIG.IsConsoleEnabled())
			INFECT_RENDERER.InitConsole();
		if (!INFECT_RENDERER.InitWindow(hInstance, nCmdShow, INFECT_GAME_CONFIG.WindowSettings()))
			std::cout << "DIRECTX DID NOT INITIALIZE PROPERLY." << std::endl;
		//if (!INFECT_RENDERER.LoadShader("base3D.hlsl"))  // TODO: MOVE THIS
		//	return 1;
		if (!INFECT_RENDERER.LoadShader("deferred.hlsl"))  // TODO: MOVE THIS
			return 1;
		if (!INFECT_RENDERER.LoadShader("quadRender.hlsl"))  // TODO: MOVE THIS
			return 1;
		if (!INFECT_RENDERER.LoadShader("deferredFinal.hlsl"))  // TODO: MOVE THIS
			return 1;
		if (!INFECT_RENDERER.LoadShader("shadowCastingLight.hlsl"))  // TODO: MOVE THIS
			return 1;
		if (!INFECT_RENDERER.LoadShader("shadowAddLight.hlsl"))  // TODO: MOVE THIS
			return 1;
		if (!INFECT_RENDERER.LoadShader("GaussianBlur.hlsl"))  // TODO: MOVE THIS
			return 1;
        if (!INFECT_RENDERER.LoadShader("ShadowMapBlur.hlsl"))  // TODO: MOVE THIS
            return 1;
        if (!INFECT_RENDERER.LoadShader("ShadowMapBlur.hlsl"))  // TODO: MOVE THIS
            return 1;

		INFECT_INPUT.Init(hInstance);
		INFECT_RESOURCES.Init();

		//Audio INIT and Load Sounds
		INFECT_AUDIOMANAGER.Init();
																   //3D	//loop //Audio-Stream
		INFECT_AUDIOMANAGER.LoadSound(R"(ASSETS/SOUNDS/rosey.wav)", true, true, false);
		INFECT_AUDIOMANAGER.LoadSound(R"(ASSETS/SOUNDS/Blackouts.mp3)", true, false, false);


		INFECT_CMC.RegisterCompMngr(new TransformComponentManager());
		INFECT_CMC.RegisterCompMngr(new CameraComponentManager());
		INFECT_CMC.RegisterCompMngr(new PointLightComponentManager());
		INFECT_CMC.RegisterCompMngr(new DirectionalLightComponentManager());
		INFECT_CMC.RegisterCompMngr(new MeshComponentManager());
		INFECT_CMC.RegisterCompMngr(new ParticleEmitterComponentManager());
		INFECT_CMC.RegisterCompMngr(new RigidBodyComponentManager());
		INFECT_CMC.RegisterCompMngr(new FractureComponentManager());
		INFECT_CMC.RegisterCompMngr(new BrainComponentManager());
		INFECT_CMC.RegisterCompMngr(new ControllerComponentManager());
		INFECT_CMC.Init();
		INFECT_MEMORY.LateInit();

		INFECT_PHYSICS.Init();

		INFECT_THREAD_JOBS.RegisterThreadContainer(new RenderThreadContainer());
		INFECT_THREAD_JOBS.RegisterThreadContainer(new SimulationThreadContainer());
		return 0;
	}

	MSG StartGameLoop()
	{
		return INFECT_GAME_STATE.Update(); // Start game loop
	}

	float GetFrameTime()
	{
		return INFECT_FRAMERATE.GetFrameTime();
	}

	void FrameStart()
	{
		INFECT_FRAMERATE.FrameStart();
		
	}

	// NO LONGER IN USE
	void Update(float deltaTime)
	{
		INFECT_INPUT.Update();							// Update input keys
		//INFECT_DEBUG.Update();						// Toggles debug drawing if needed
		INFECT_EVENTS.Update(deltaTime);				// Pump the event manager
		//INFECT_AUDIO.Update(deltaTime);

		INFECT_CMC.Update(deltaTime);					// Updates all ComponentManagers



		//INFECT_GOM.UpdateStatus();					// Update status of game objects
		//INFECT_PHYSICS.Integrate(deltaTime);			// Move physics bodies
		//INFECT_PHYSICS.ResolveCollisions();			// Resolve collisions on physics bodies

		//INFECT_RENDERER.RenderFrame(pGOCamera, pGO);


		INFECT_THREAD_JOBS.AddNewJob(new StartRenderCommand(*INFECT_THREAD_JOBS.GetThreadContainer<RenderThreadContainer>(ThreadType::RenderThread)));
		//INFECT_GOM.RenderCameras();						// Render all game objects
		//INFECT_IMGUI.Update();						// Update all Imgui commands

		INFECT_PHYSICS.Update(deltaTime);              // Update _PHYSICS
	}

	void FrameEnd()
	{
		//INFECT_IMGUI.FrameEnd();						// Render Imgui commands
		INFECT_FRAMERATE.FrameEnd();					// Lock FPS 
	}

	void LoadPrefabs(std::string fileName)
	{

	}

	void UnloadResources()
	{
		INFECT_RESOURCES.UnloadAll();
	}

	void Cleanup()
	{
		INFECT_RENDERER.DestroyConsole();
	}
}
