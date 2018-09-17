/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

namespace Infect {

	int Initialize(std::string configFile, HINSTANCE hInstance, int nCmdShow)
	{
		INFECT_EVENTS.Init();
		INFECT_GAME_CONFIG.LoadConfig(configFile);
		INFECT_GOM.Init();
		if (INFECT_GAME_CONFIG.IsConsoleEnabled())
			INFECT_RENDERER.InitConsole();
		if (!INFECT_RENDERER.InitWindow(hInstance, nCmdShow, INFECT_GAME_CONFIG.WindowSettings()))
			std::cout << "DIRECTX DID NOT INITIALIZE PROPERLY." << std::endl;
		if (!INFECT_RENDERER.LoadShader())  // TODO: MOVE THIS
			return 1;
		INFECT_INPUT.Init(hInstance);
		INFECT_RESOURCES.Init();


		INFECT_CMC.RegisterCompMngr(new TransformComponentManager());
		INFECT_CMC.RegisterCompMngr(new CameraComponentManager());
		INFECT_CMC.RegisterCompMngr(new PointLightComponentManager());
		INFECT_CMC.RegisterCompMngr(new MeshComponentManager());
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
		INFECT_RENDERER.FrameStart();
	}

	void Update(float deltaTime)
	{
		INFECT_INPUT.Update();									// Update input keys
		//INFECT_DEBUG.Update();									// Toggles debug drawing if needed
		INFECT_EVENTS.Update(deltaTime);							// Pump the event manager
		//INFECT_AUDIO.Update(deltaTime);

		INFECT_GOM.Update(deltaTime);					// Update game logic
		INFECT_CMC.Update(deltaTime);



		//INFECT_GOM.UpdateStatus();						// Update status of game objects
		//INFECT_PHYSICS.Integrate(deltaTime);						// Move physics bodies
		//INFECT_PHYSICS.ResolveCollisions();						// Resolve collisions on physics bodies
		INFECT_GOM.LateUpdate(deltaTime);				// Update game logic that occurs after physics

		//INFECT_RENDERER.RenderFrame(pGOCamera, pGO);


		INFECT_GOM.RenderCameras();					// Render all game objects
		//INFECT_IMGUI.Update();									// Update all Imgui commands
	}

	void FrameEnd()
	{
		//INFECT_IMGUI.FrameEnd();									// Render Imgui commands
		INFECT_RENDERER.FrameEnd();								// Swap window buffer
		INFECT_FRAMERATE.FrameEnd();								// Lock FPS 
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
