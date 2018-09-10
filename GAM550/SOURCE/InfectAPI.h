/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef INFECT_API_H
#define INFECT_API_H

namespace Infect {
	//! int Initialize(std::string configFileName)
	/*!
	\param configFileName The config json file to use for setup, must include file path
	\param hInstance
	\param nCmdShow
	\return Error code: 0 - everything went well, non-zero - something went wrong
	*/
	int Initialize(std::string configFileName, HINSTANCE hInstance, int nCmdShow);

	//! MSG StartGameLoop()
	/*!
	Calls the Game State to begin the engine game loop
	\return MSG: Windows message
	*/
	MSG StartGameLoop();

	float GetFrameTime();

	//! double FrameStart()
	/*!
	Starts the game frame. Calls various start and update methods on managers.
	*/
	void FrameStart();

	//! void Update(double deltaTime)
	/*!
	Calls various update methods on managers
	\param deltaTime - Duration of the previous frame
	*/
	void Update(float deltaTime);

	//! void FrameEnd()
	/*!
	Ends the game frame. Calls various end and update methods on managers.
	*/
	void FrameEnd();

	//! void LoadGameObjects(std::string fileName)
	/*!
	Parses a given json file which in turn should list other json files each containing a game object archetype.
	Each archetype is loaded into the engine for use in creation later.
	\param fileName - The json file containing what game object archetypess to load, must include file path
	*/
	void LoadPrefabs(std::string filePath);

	//! void UnloadResources()
	/*!
	Calls to unload all currently loaded resources. Meshes, textures, game object archetypes, etc.
	*/
	void UnloadResources();

	//! void Cleanup()
	/*!
	Calls all appropriate cleanup functions for the engine's managers.
	*/
	void Cleanup();
}

#endif
