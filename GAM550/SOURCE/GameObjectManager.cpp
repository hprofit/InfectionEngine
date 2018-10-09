/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit, Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include "Stdafx.h"

bool GameObjectManager::_HasShadowCastingLight(const GameObject & lightGO)
{
	return lightGO.HasComponent(ComponentType::C_DirectionalLight);;
}

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
	//for (int i = 0; i < mp_GameObjects.size(); ++i) {
	//	if (mp_GameObjects[i])
	//		INFECT_MEMORY.DeleteGameObject(mp_GameObjects[i]);//delete mp_GameObjects[i];
	//}
}

GameObject * GameObjectManager::SpawnGameObject(const std::string prefabName)
{
	json* j = INFECT_RESOURCES.GetPrefabFile(prefabName + ".json");
	if (!j) return nullptr;

	GameObject* newGO = INFECT_MEMORY.GetNewGameObject();
	newGO->SwitchTag(TAG_NAMES[ParseString(*j, "Tag")]);

	unsigned totalActiveComp = static_cast<unsigned>((*j)[COMPONENTS].size());
	for (unsigned i = 0; i < totalActiveComp; i++) {
		Component* pComponent = INFECT_MEMORY.GetNewComponent(COMPONENT_NAMES[ParseString((*j)[COMPONENTS][i], "Component")]);
		newGO->AddComponent(pComponent);
		pComponent->Serialize((*j)[COMPONENTS][i]);
	}

	newGO->LateInitialize();

	mp_GameObjects.push_back(newGO); // is this necessary?
	return newGO;
}

GameObject* GameObjectManager::SpawnGameObject() {
	GameObject* newGO = INFECT_MEMORY.GetNewGameObject();
	newGO->LateInitialize();
	mp_GameObjects.push_back(newGO); // is this necessary?
	return newGO;
}

void GameObjectManager::RegisterCamera(GameObject * cameraGO)
{
	mp_Cameras.push_back(cameraGO);
	Sorting::InsertionSort(mp_Cameras, &CameraComponent::LeftDepthGreaterThanRight);
}

void GameObjectManager::RegisterLight(GameObject * lightGO)
{
	mp_Lights.push_back(lightGO);
}

void GameObjectManager::RegisterShadowCastingLight(GameObject * lightGO)
{
	mp_ShadowCastingLights.push_back(lightGO);
}

void GameObjectManager::UnregisterLight(GameObject * lightGO)
{
	mp_Lights.erase(
		std::remove(mp_Lights.begin(), mp_Lights.end(), lightGO),
		mp_Lights.end()
	);
}

void GameObjectManager::UnregisterShadowCastingLight(GameObject * lightGO)
{
	mp_ShadowCastingLights.erase(
		std::remove(mp_ShadowCastingLights.begin(), mp_ShadowCastingLights.end(), lightGO),
		mp_ShadowCastingLights.end()
	);
}

void GameObjectManager::RenderCameras()
{
	for (unsigned int camIdx = 0; camIdx < mp_Cameras.size(); ++camIdx) {
		for (unsigned int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
			if (mp_GameObjects[objIdx]->IsActive() && mp_Cameras[camIdx] != mp_GameObjects[objIdx])
				INFECT_RENDERER.RenderObject((*mp_Cameras[camIdx]), (*mp_GameObjects[objIdx]));
		}
	}
}

void GameObjectManager::RenderLights()
{
	for (unsigned int camIdx = 0; camIdx < mp_Cameras.size(); ++camIdx) {
		for (unsigned int lightIdx = 0; lightIdx < mp_Lights.size(); ++lightIdx) {
			if (mp_Lights[lightIdx]->IsActive() && 
				mp_Cameras[camIdx] != mp_Lights[lightIdx])
				INFECT_RENDERER.RenderLight((*mp_Cameras[camIdx]), (*mp_Lights[lightIdx]));
		}
	}
}

void GameObjectManager::RenderShadowCastingLights()
{
	for (unsigned int lightIdx = 0; lightIdx < mp_ShadowCastingLights.size(); ++lightIdx) {
		if (!mp_ShadowCastingLights[lightIdx]->IsActive())	continue;
		DirectionalLightComponent* pDLComp = mp_ShadowCastingLights[lightIdx]->GetComponent<DirectionalLightComponent>();

		// Clear this light's render target from previous frame
		pDLComp->ClearRenderTarget();

		// Bind this light's render target for rendering to it's depth buffer
		pDLComp->BindRenderTarget();

		// Render all objects to fill out light's depth buffer
		for (unsigned int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
			if (mp_GameObjects[objIdx]->IsActive() && mp_ShadowCastingLights[lightIdx] != mp_GameObjects[objIdx])
				INFECT_RENDERER.RenderShadowCastingLight((*mp_ShadowCastingLights[lightIdx]), (*mp_GameObjects[objIdx]));
		}
	}
}

void GameObjectManager::RenderParticles()
{
	for (unsigned int camIdx = 0; camIdx < mp_Cameras.size(); ++camIdx) {
		for (unsigned int objIdx = 0; objIdx < mp_GameObjects.size(); ++objIdx) {
			if (mp_GameObjects[objIdx]->IsActive() && mp_Cameras[camIdx] != mp_GameObjects[objIdx])
				INFECT_RENDERER.RenderObject((*mp_Cameras[camIdx]), (*mp_GameObjects[objIdx]));
		}
	}
}
