/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.h
Purpose: Loads Meshes and other commonly used game resources into
memory and manages their instances as well as returns references to them.
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


class ResourceManager
{
private:
	Assimp::Importer m_importer;
	
	std::unordered_map<std::string, Mesh* > m_meshes;
	std::unordered_map<std::string, Scene* > m_scenes;		

	// Parses a file using assimp and returns a pointer to the created Scene object
	Scene* _LoadScene(const std::string& sceneName);
public:
	ResourceManager();
	~ResourceManager();
	ResourceManager(const ResourceManager &) = delete;
	void operator=(const ResourceManager &) = delete;

	bool Init();

	// Returns a pointer to a Scene object
	Scene* GetScene(const std::string& sceneName);
	// Deletes the ResourceManager's reference to the specified Scene object
	void UnloadMesh(const std::string& sceneName);
	// Deletes the Resourcemanager's reference to all resource objects
	void UnloadAll();
};

#endif