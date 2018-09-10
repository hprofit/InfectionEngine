#include "Stdafx.h"

GameObjectManager::GameObjectManager() : m_GUID(0)
{
	for (int i = 0; i < MAX_GAMEOBJECTS; ++i) {
		mp_GameObjects[i] = new GameObject(i);
	}
}

GameObjectManager::~GameObjectManager()
{
	for (int i = 0; i < MAX_GAMEOBJECTS; ++i) {
		if (mp_GameObjects[i])
			delete mp_GameObjects[i];
	}
}

GameObject * GameObjectManager::SpawnGameObject()
{
	GameObject* newGO = new GameObject(m_GUID++);
	mp_GameObjects.push_back(newGO);
	return newGO;
}

void GameObjectManager::RegisterCamera(GameObject * cameraGO)
{
	mp_Cameras.push_back(cameraGO);
	Sorting::InsertionSort(mp_Cameras, &CameraComponent::LeftDepthGreaterThanRight);
}

void GameObjectManager::RenderCameras()
{
	for (int camIdx = 0; camIdx < mp_Cameras.size(); ++camIdx) {
		for (int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
			if (mp_GameObjects[objIdx]->IsActive())
				INFECT_RENDERER.RenderObject(mp_Cameras[camIdx], mp_GameObjects[objIdx]);
		}
	}
}

