/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#define MAX_GAMEOBJECTS 2//2048

class GameObjectManager : public Subscriber
{
private:
	GameObject * mp_GameObjects[MAX_GAMEOBJECTS];

public:
	GameObjectManager();
	~GameObjectManager();

	void Init() {};

	GameObject* GetGameObject(unsigned int id) const { return mp_GameObjects[id]; }
};

#endif

