/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

#define GAME_SETTINGS "GAME_SETTINGS"
#define RENDER_SETTINGS "RENDER_SETTINGS"
#define INPUT_SETTINGS "INPUT_SETTINGS"
#define WINDOW_SETTINGS "WINDOW_SETTINGS"

GameConfig::GameConfig() : m_consoleEnabled(false){}

GameConfig::~GameConfig() {}

void GameConfig::LoadConfig(std::string s) {
	json j = OpenJsonFile(s);
	json gameSettings = j[GAME_SETTINGS];
	json renderSettings = j[RENDER_SETTINGS];
	json windowSettings = j[WINDOW_SETTINGS];

	m_consoleEnabled = ParseBool(renderSettings, "consoleEnabled");

	std::string assetDirectory = ParseString(gameSettings, "assetDirectory");

	// Set project directories
	m_levelFilesDir = assetDirectory + ParseString(gameSettings, "levelFilesDir");
	m_prefabsDir = assetDirectory + ParseString(gameSettings, "prefabsDir");
	m_audioDir = assetDirectory + ParseString(gameSettings, "audioDir");
	m_shadersDir = assetDirectory + ParseString(gameSettings, "shadersDir");
	m_texturesDir = assetDirectory + ParseString(gameSettings, "texturesDir");
	m_roomFilesDir = assetDirectory + ParseString(gameSettings, "roomFilesDir");
	m_fontsDir = assetDirectory + ParseString(gameSettings, "fontsDir");
	m_meshesDir = assetDirectory + ParseString(gameSettings, "meshesDir");

	m_debugEnabled = ParseBool(gameSettings, "debugMode");

	//set mute
	m_soundsMute = ParseBool(gameSettings,"soundsMute");

	m_windowSettings.Width = ParseInt(windowSettings, "width");
	m_windowSettings.Height = ParseInt(windowSettings, "height");
	m_windowSettings.FullScreen = ParseBool(windowSettings, "fullScreen");
	m_windowSettings.WindowTitle = ParseString(windowSettings, "title");
	m_windowSettings.VSync = ParseBool(windowSettings, "vsync");

	//if (ParseBool(renderSettings, "enableWindowsCursor"))
	//	INFECT_RENDERER.EnableWindowsCursor();
	//else
	//	INFECT_RENDERER.DisableWindowsCursor();

}

#undef GAME_SETTINGS 
#undef RENDER_SETTINGS
#undef INPUT_SETTINGS 
#undef WINDOW_SETTINGS