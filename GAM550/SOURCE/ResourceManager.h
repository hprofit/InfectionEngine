/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
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
	std::unordered_map<std::string, ID3D11ShaderResourceView* > m_textures;
	std::unordered_map<std::string, json*> m_prefabs;

	// Parses a file using assimp and returns a pointer to the created Scene object
	Scene* _LoadScene(const std::string& sceneName);

	ID3D11ShaderResourceView* _LoadTexture(const std::string& textureName);
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
	// Returns a pointer to a texture object
	ID3D11ShaderResourceView* GetTexture(const std::string& textureName);
	// Deletes the ResourceManager's reference to the specified texture object
	void UnloadTexture(const std::string& textureName);
	// Returns a pointer to a json prefab data
	json* GetPrefabFile(const std::string& prefabName);
	// Deletes the Resourcemanager's reference to all resource objects
	void UnloadAll();
};

#endif