#include "Stdafx.h"

GameObjectManager::GameObjectManager()
{
	mp_GameObjects[0] = new GameObject(0);
}

GameObjectManager::~GameObjectManager()
{
	for (int i = 0; i < MAX_GAMEOBJECTS; ++i) {
		if (mp_GameObjects[i])
			delete mp_GameObjects[i];
	}
}

