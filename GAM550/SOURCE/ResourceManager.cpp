/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

namespace fs = std::experimental::filesystem;

ResourceManager::ResourceManager(){}

ResourceManager::~ResourceManager() 
{
	UnloadAll();
}

bool ResourceManager::Init()
{
	//LoadPrefabFiles();

	Plane* plane = new Plane();
	plane->FinishMesh();
	Scene* pScenePlane = new Scene(1);
	(*pScenePlane)[0] = plane;
	m_scenes[PLANE_PRIMITIVE] = pScenePlane;

	Cube* pCube = new Cube();
	m_scenes[CUBE_PRIMITIVE] = pCube;

	Sphere* pSphere = new Sphere(40);
	pSphere->FinishMesh();
	Scene* pSceneSphere = new Scene(1);
	(*pSceneSphere)[0] = pSphere;
	m_scenes[SPHERE_PRIMITIVE] = pSceneSphere;

	PolarSphere* pPolarSphere = new PolarSphere();
	pPolarSphere->FinishMesh();
	Scene* pScenePolarSphere = new Scene(1);
	(*pScenePolarSphere)[0] = pPolarSphere;
	m_scenes[POLAR_SPHERE_PRIMITIVE] = pScenePolarSphere;

	return true;
}

#pragma region Mesh

Scene* ResourceManager::_LoadScene(const std::string& meshSceneName)
{
	const aiScene* scene = m_importer.ReadFile(/*INFECT_GAME_CONFIG.MeshesDir()*/"ASSETS/MESHES/" + meshSceneName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);

	if (scene->HasMeshes()) {
		Scene* meshScene = new Scene(scene->mNumMeshes);
		m_scenes[meshSceneName] = meshScene;
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* aiMeshPtr = scene->mMeshes[i];
			m_meshes[aiMeshPtr->mName.C_Str()] = new Mesh(aiMeshPtr);
			(*meshScene)[i] = m_meshes[aiMeshPtr->mName.C_Str()];
		}
		return meshScene;
	}
	else
		return nullptr;
}

Scene* ResourceManager::GetScene(const std::string& meshSceneName)
{
	Scene* meshScene = m_scenes[meshSceneName];

	if (meshScene)
		return meshScene;
	else
		return _LoadScene(meshSceneName);
}

void ResourceManager::UnloadMesh(const std::string& meshName)
{
	if (m_meshes[meshName]) {
		delete m_meshes[meshName];
		m_meshes.erase(meshName);
	}
}

#pragma endregion

void ResourceManager::UnloadAll()
{
	for (auto comp : m_meshes) {
		if (comp.second)
			delete comp.second;
	}
	m_meshes.clear();
}
