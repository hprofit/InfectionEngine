/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

GameStateManager::GameStateManager() :
	m_currentState(GameState::CURRENT_LEVEL),
	m_nextState(GameState::CURRENT_LEVEL),
	m_debugPause(false), m_isGamePaused(false), m_isLevelOver(false)
{

}

GameStateManager::~GameStateManager() {}

MSG GameStateManager::Update() {
	// this struct holds Windows event messages
	MSG msg;
	INFECT_LEVEL.LoadLevel(0); // Loading First Level

	while (m_currentState != GameState::QUIT) {
  
		m_currentState = GameState::CURRENT_LEVEL;
		m_nextState = GameState::CURRENT_LEVEL;
		
		INFECT_THREAD_JOBS.AddNewJob(new StartSimulationCommand(*INFECT_THREAD_JOBS.GetThreadContainer<SimulationThreadContainer>(ThreadType::SimThread)));

		// Game loop
		// wait for the next message in the queue, store the result in 'msg'
		while (m_currentState == m_nextState) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				// translate keystroke messages into the right format
				TranslateMessage(&msg);
				// send the message to the WindowProc function
				DispatchMessage(&msg);
			}
		}

		m_currentState = m_nextState;
	}

	Infect::UnloadResources();	// Unloads all resources
	Infect::Cleanup();

	return msg;
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
			if (m_isLevelOver) return;
			InputButtonData* pData = p_event->Data<InputButtonData>();
			if (pData->m_isTrigger) {
				PauseGame(!m_isGamePaused);
				if(m_isGamePaused) INFECT_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnPauseGame));
				else INFECT_EVENTS.BroadcastEventToSubscribers(&Event(EVENT_OnGameResume));
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
	}
}

void GameStateManager::SetGameState(GameState state) { 
	m_nextState = state; 
	PauseGame(false);
	m_isLevelOver = false;
}