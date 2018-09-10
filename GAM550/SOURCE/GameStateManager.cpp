/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

GameStateManager::GameStateManager() :
	m_currentState(GameState::CURRENT_LEVEL),
	m_nextState(GameState::CURRENT_LEVEL),
	m_debugPause(false),
{

}

GameStateManager::~GameStateManager() {}

void GameStateManager::Update() {
	while (m_currentState != GameState::QUIT) {

		m_currentState = GameState::CURRENT_LEVEL;
		m_nextState = GameState::CURRENT_LEVEL;

		// Game loop
		while (m_currentState == m_nextState) {

			Infect::FrameStart();

			Infect::Update(Infect::GetFrameTime());			// Game loop

			Infect::FrameEnd();
		}

		m_currentState = m_nextState;
	}

	Infect::UnloadResources();	// Unloads all resources
}

void GameStateManager::HandleEvent(Event * p_event) {
	switch (p_event->Type()) {
		case EVENT_WINDOW_CLOSED: {
			SetGameState(GameState::QUIT);
			break;
		}
		case RESTART_LEVEL: {
			SetGameState(GameState::RESTART);
			break;
		}
		case EVENT_INPUT_PAUSEGAME: {
			if (m_isLevelOver || m_isShopOpen || m_isViewingCredits || !TETRA_UI.IsCanvasAvailable(CanvasType::CANVAS_PAUSE)) return;
			InputButtonData* pData = p_event->Data<InputButtonData>();
			if (pData->m_isTrigger) {
				PauseGame(!m_isGamePaused);
				if(m_isGamePaused) TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnPauseGame));
				else TETRA_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnGameResume));
			}
			break;
		}
		case EVENT_LevelInComplete: {
			m_isLevelOver = true;
			PauseGame(true);
			break;
		}
		case EVENT_ExitLevel: {
			m_isLevelOver = true;
			break;
		}
		case EVENT_ShopOpened: {
			m_isShopOpen = true;
			break;
		}
		case EVENT_ShopClosed: {
			m_isShopOpen = false;
			break;
		}
	}
}

void GameStateManager::SetGameState(GameState state) { 
	m_nextState = state; 
	PauseGame(false);
	m_isLevelOver = false;
}

void GameStateManager::ActivateCheats() {
	if (TETRA_INPUT.IsKeyPressed(SDL_SCANCODE_C) && TETRA_INPUT.IsKeyPressed(SDL_SCANCODE_H) && TETRA_INPUT.IsKeyPressed(SDL_SCANCODE_E)) {
		m_isCheatsActive = true;
	}
}
