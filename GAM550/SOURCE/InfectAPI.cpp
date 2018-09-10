/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

namespace Infect {

	int Initialize(std::string configFile)
	{
		INFECT_RESOURCES.Init();

		return 0;
	}

	void StartGameLoop()
	{
		INFECT_GAME_STATE.Update(); // Start game loop
	}

	float GetFrameTime()
	{
		//return INFECT_FRAMERATE.GetFrameTime();
	}

	void FrameStart()
	{
		//INFECT_FRAMERATE.FrameStart();
		INFECT_RENDERER.FrameStart();
	}

	void Update(float deltaTime)
	{
		//INFECT_INPUT.Update();									// Update input keys
		//INFECT_DEBUG.Update();									// Toggles debug drawing if needed
		INFECT_EVENTS.Update(deltaTime);							// Pump the event manager
		//INFECT_AUDIO.Update(deltaTime);
		//INFECT_GAME_OBJECTS.Update(deltaTime);					// Update game logic
		//INFECT_GAME_OBJECTS.UpdateStatus();						// Update status of game objects
		//INFECT_PHYSICS.Integrate(deltaTime);						// Move physics bodies
		//INFECT_PHYSICS.ResolveCollisions();						// Resolve collisions on physics bodies
		//INFECT_GAME_OBJECTS.LateUpdate(deltaTime);				// Update game logic that occurs after physics

		//INFECT_GAME_OBJECTS.RenderGameObjects();					// Render all game objects
		//INFECT_IMGUI.Update();									// Update all Imgui commands
	}

	void FrameEnd()
	{
		//INFECT_IMGUI.FrameEnd();									// Render Imgui commands
		INFECT_RENDERER.FrameEnd();								// Swap window buffer
		//INFECT_FRAMERATE.FrameEnd();								// Lock FPS 
		// TODO: Clean up GameObjects
	}

	void UnloadResources()
	{
		INFECT_RESOURCES.UnloadAll();
	}

	void LoadPrefabs(std::string fileName)
	{

	}
}
