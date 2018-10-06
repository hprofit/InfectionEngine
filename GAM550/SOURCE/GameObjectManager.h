/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#define MAX_GAMEOBJECTS 2048

class GameObjectManager : public Subscriber
{
private:
	std::vector<GameObject *> mp_GameObjects;
	std::vector<GameObject *> mp_Cameras;
	std::vector<GameObject *> mp_Lights;

public:
	GameObjectManager();
	~GameObjectManager();

	void Init() {};

	GameObject* SpawnGameObject(const std::string prefabName);
	GameObject* SpawnGameObject();

	GameObject* GetGameObject(unsigned int id) const { return mp_GameObjects[id]; }

	GameObject* GetCamera() const { return mp_Cameras[0]; }

	void RegisterCamera(GameObject* cameraGO);

	void RegisterLight(GameObject* lightGO);

	void UnregisterLight(GameObject* lightGO);

	void RenderCameras();

	void RenderLights();

	// TODO: Remove this
	void RenderParticles();

	void Update(float deltaTime);

	void LateUpdate(float deltaTime);
};

#endif

