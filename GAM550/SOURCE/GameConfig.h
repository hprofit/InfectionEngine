/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

class GameConfig
{
protected:
	std::string m_texturesDir;
	std::string m_levelFilesDir;
	std::string m_prefabsDir;
	std::string m_audioDir;
	std::string m_shadersDir;
	std::string m_roomFilesDir;
	std::string m_fontsDir;
	std::string m_meshesDir;
	
	bool m_soundsMute;

	bool m_consoleEnabled;
	bool m_debugEnabled;
	bool m_postProcessingEnabled;
	WindowSettings m_windowSettings;

public:
	GameConfig();
	~GameConfig();
	GameConfig(const GameConfig &) = delete;
	void operator=(const GameConfig &) = delete;

	void LoadConfig(std::string);

	inline std::string TexturesDir() const { return m_texturesDir; }
	inline std::string LevelFilesDir() const { return m_levelFilesDir; }
	inline std::string PrefabsDir() const { return m_prefabsDir; }
	inline std::string AudioDir() const { return m_audioDir; }
	inline std::string ShadersDir() const { return m_shadersDir; }
	inline std::string RoomFilesDir() const { return m_roomFilesDir; }
	inline std::string FontsDir() const { return m_fontsDir; }
	inline std::string MeshesDir() const { return m_meshesDir; }
	inline WindowSettings WindowSettings() const { return m_windowSettings; }

	inline bool GetsoundsMute(){ return m_soundsMute; }

	inline bool IsConsoleEnabled() const { return m_consoleEnabled; }
};

#endif