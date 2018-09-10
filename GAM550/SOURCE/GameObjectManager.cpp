#include "Stdafx.h"

GameObjectManager::GameObjectManager()
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

