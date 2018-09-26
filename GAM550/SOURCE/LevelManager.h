/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

class LevelManager: public Subscriber
{
private:
	unsigned m_currentLvl;
	unsigned m_levelSize;
	json levelConfig;
public:
	LevelManager();
	~LevelManager();
	LevelManager(const LevelManager &) = delete;
	void operator=(const LevelManager &) = delete;

	void Init(const json& j);
	void LoadLevel(unsigned levelNum);
	void UnLoadLevel();
	void ChangeLevel();
	void NextLevel();
	void HandleEvent(Event* pEvent);
};

#endif