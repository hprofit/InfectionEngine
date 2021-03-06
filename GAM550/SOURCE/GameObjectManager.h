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
	std::vector<GameObject *> mp_ShadowCastingLights;

	bool _HasShadowCastingLight(const GameObject& lightGO);
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

	void RegisterShadowCastingLight(GameObject* lightGO);
	void UnregisterShadowCastingLight(GameObject* lightGO);
	GameObject* GetShadowCastingLight(UINT index) { return mp_ShadowCastingLights[index]; }

	// Cycles through all cameras and renders all objects in the scene to the currently
	// bound buffer using the currently selected shader
	void RenderCameras();

	// Cycles through all non-shadow casting lights and adds their influence to the deferred + ambient
	// lit scene based on their geometry
	void RenderLights();

	// Cycles through all shadow casting lights and binds their render targets for drawing
	// Each visible object is then drawn to the light's shadpw map/depth buffer
	void RenderShadowCastingLights();

	// Cycles through all shadow casting lights and adds their influence to the deferred + ambient
	// lit scene if an object is in within it's line of sight
	void AddLightFromShadowCastingLights();

	// TODO: Remove this
	void RenderParticles();
};

#endif

