/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#include <Stdafx.h>

static const std::string GAME_OBJECTS = "GAME_OBJECTS";

LevelManager::LevelManager(): m_currentLvl(0), m_levelSize(0) {
	INFECT_EVENTS.Subscribe(EVENT_INPUT_RESTART, this);

}

LevelManager::~LevelManager() {}

void LevelManager::Init(const json& config){
	this->levelConfig = config;
	m_levelSize = static_cast<unsigned>(levelConfig["Levels"].size());
	m_currentLvl = 0; // level 0
}

void LevelManager::LoadLevel(unsigned levelNum){
	json js = OpenJsonFile(INFECT_GAME_CONFIG.LevelFilesDir() + ParseString(levelConfig["Levels"][levelNum], "Name") + ".json");

	size_t gameObjectSize = js["GAME_OBJECTS"].size();
	for (unsigned i = 0; i < gameObjectSize; i++) {
		GameObject* pGO = INFECT_GOM.SpawnGameObject(js[GAME_OBJECTS][i]["prefab"]);
		if (pGO) {
			pGO->OverrideComponents(js[GAME_OBJECTS][i]);
		}
	}
}


void LevelManager::UnLoadLevel(){

}

void LevelManager::ChangeLevel(){

}

void LevelManager::NextLevel(){

}

void LevelManager::HandleEvent(Event* pEvent){
	switch (pEvent->Type()) {
		case EVENT_INPUT_RESTART: {
			break;
		}
	}
}