/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>
#include <iostream>

#define JoystickDeadZone 5500
#define SAFERELEASE(p) {if(p) {(p)->Release(); (p)=nullptr;}}
#define SAFEDELETE(p)  {if(p) {delete (p); (p)=nullptr;}}

InputManager::InputManager()
	:	m_PrevLeftMouse(false), m_LeftMouse(false), 
		m_PrevRightMouse(false), m_RightMouse(false), m_isJoystickControlsActive(false) {
	
	// initialize mouse pos
	m_MousePosX = 0;
	m_MousePosY = 0;
	// initialize keyboard states
	m_PreviousKeyStates = new Uint8[256];
	m_CurrentKeyStates = new Uint8[256];
	memset(m_PreviousKeyStates, 0, 256 * sizeof(Uint8));
	memset(m_CurrentKeyStates, 0, 256 * sizeof(Uint8));
	// initialize gamecontroller button states
	m_PreviousButtonStates = new Uint8[XBOX_NUM_SCANCODES];
	m_CurrentButtonStates = new Uint8[XBOX_NUM_SCANCODES];
	memset(m_PreviousKeyStates, 0, XBOX_NUM_SCANCODES * sizeof(Uint8));
	memset(m_CurrentKeyStates, 0, XBOX_NUM_SCANCODES * sizeof(Uint8));
	m_StickRightX, m_StickLeftX = 0;
	m_StickRightY, m_StickLeftY = 0;

	// initialize for SDL_GameController
	/*if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
		printf( "ERROR: SDL_Init() with SDL_INIT_JOYSTICK flag failed!\n");
	}*/

	//TETRA_EVENTS.Subscribe(EVENT_INPUT_TOGGLEJOYSTICK, this);
}

InputManager::~InputManager() {
	// delete key states
	delete m_CurrentKeyStates;
	delete m_PreviousKeyStates;			
	//SDL_GameControllerClose(mGameController);
	delete m_CurrentButtonStates;
	delete m_PreviousButtonStates;

	for (auto command : m_inputCommands) {
		delete command;
	}
	m_inputCommands.clear();
	FreeDirectInput();
}

void InputManager::FreeDirectInput() {
	// Un-acquire before release
	if (mDIRX_Mouse) 
		mDIRX_Mouse->Unacquire();
	if (mDIRX_Keyboard) 
		mDIRX_Keyboard->Unacquire();
	// Release the main interface to direct input.
	SAFERELEASE(mDIRX_Keyboard);
	SAFERELEASE(mDIRX_Mouse);
	SAFERELEASE(DIRX_Interface);
}

void InputManager::Init(HINSTANCE hInstance) {
	// Connecting DirectInput Interface
	DirectInput8Create(
		GetModuleHandle(NULL),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DIRX_Interface,
		NULL);
	// DirectInput Connection Failed
	if (DIRX_Interface == NULL) {
		MessageBox(NULL, TEXT("DirectInput Interface Connection Failed"),
					TEXT("InputManager: Init()"), MB_ICONERROR | MB_OK);
		return;
	}
	// Creating Keyboard Device
	DIRX_Interface->CreateDevice(GUID_SysKeyboard, &mDIRX_Keyboard, NULL);
	if (mDIRX_Keyboard) {
		mDIRX_Keyboard->SetDataFormat(&c_dfDIKeyboard);
		mDIRX_Keyboard->SetCooperativeLevel(INFECT_RENDERER.GethWnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		mDIRX_Keyboard->Acquire();
	}
	else {
		MessageBox(NULL, TEXT("DirectInput keyboard init failed"),
					TEXT("InputManager: Init()"), MB_ICONERROR | MB_OK);
		return;
	}
	// Creating Mouse Device
	DIRX_Interface->CreateDevice(GUID_SysMouse, &mDIRX_Mouse, NULL);
	if (mDIRX_Mouse) {
		mDIRX_Mouse->SetDataFormat(&c_dfDIMouse);
		mDIRX_Mouse->SetCooperativeLevel(INFECT_RENDERER.GethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		mDIRX_Mouse->Acquire();
	}
	else {
		MessageBox(NULL, TEXT("DirectInput mouse init failed"),
			TEXT("InputManager: Init()"), MB_ICONERROR | MB_OK);
		return;
	}
}

void InputManager::Update() {
	HRESULT hr;
	// Update mouse position
	int mouseTempPosX = m_MousePosX;
	int mouseTempPosY = m_MousePosY;
	// Update previous mouse states
	m_PrevLeftMouse = m_LeftMouse;
	m_PrevRightMouse = m_RightMouse;
	m_PreviousMouseStates = m_CurrentMouseStates;
	// Get mouse state 
	hr = mDIRX_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_CurrentMouseStates);
	if (FAILED(hr)) {
		mDIRX_Mouse->Acquire();
	}

	m_MousePosRelX = m_MousePosX - mouseTempPosX;
	m_MousePosRelY = m_MousePosY - mouseTempPosY;

	// Update current mouse states
	m_LeftMouse = m_CurrentMouseStates.rgbButtons[(BYTE)MOUSEBTN::MOUSE_BTN_LEFT] & 0x80;
	m_RightMouse = m_CurrentMouseStates.rgbButtons[(BYTE)MOUSEBTN::MOUSE_BTN_RIGHT] & 0x80;
	
	// update PreviousKeyStates
	memcpy(m_PreviousKeyStates, m_CurrentKeyStates, 256 * sizeof(Uint8));
	//memcpy(m_PreviousButtonStates, m_CurrentButtonStates, XBOX_NUM_SCANCODES * sizeof(Uint8));

	// get new KeyStates
	Uint8 currentKeyStates[256];
	hr = mDIRX_Keyboard->GetDeviceState(sizeof(currentKeyStates), (LPVOID)&currentKeyStates);
	if (FAILED(hr)) {
		mDIRX_Keyboard->Acquire();
		memset(m_CurrentKeyStates, 0, 256 * sizeof(Uint8));
		return;
	}
	// update CurrentKeyStates
	memcpy(m_CurrentKeyStates, &currentKeyStates, 256 * sizeof(Uint8));

	//// update CurrentButtonStates // TODO Optimization: Ignore all this if joystick is not connected
	//SDL_GameController *GameController;
	//GameController = SDL_GameControllerOpen(0);
	//m_CurrentButtonStates[XBOX_BTN_A] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_A);
	//m_CurrentButtonStates[XBOX_BTN_B] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_B);
	//m_CurrentButtonStates[XBOX_BTN_X] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_X);
	//m_CurrentButtonStates[XBOX_BTN_Y] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_Y);
	//m_CurrentButtonStates[XBOX_BTN_BACK] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_BACK);
	//m_CurrentButtonStates[XBOX_BTN_GUIDE] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_GUIDE);
	//m_CurrentButtonStates[XBOX_BTN_START] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_START);
	//m_CurrentButtonStates[XBOX_BTN_LEFT_STICK] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_LEFTSTICK);
	//m_CurrentButtonStates[XBOX_BTN_RIGHT_STICK] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_RIGHTSTICK);
	//m_CurrentButtonStates[XBOX_BTN_LEFT_SHOULDER] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	//m_CurrentButtonStates[XBOX_BTN_RIGHT_SHOULDER] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	//m_CurrentButtonStates[XBOX_DPAD_UP] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_UP);
	//m_CurrentButtonStates[XBOX_DPAD_DOWN] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	//m_CurrentButtonStates[XBOX_DPAD_LEFT] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	//m_CurrentButtonStates[XBOX_DPAD_RIGHT] = SDL_GameControllerGetButton(GameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	//m_StickLeftX = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTX);
	//m_StickLeftY = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY);
	//m_StickRightX = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX);
	//m_StickRightY = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTY);

	//FireEvents();
	
}

void InputManager::FireEvents() {
	for (auto command : m_inputCommands) {
		switch (command->m_inputType) {
			case InputType_Button: {
				bool isTrigger = false, isRelease = false, isPressed = false;
				if (command->m_isJoystick && command->m_isMouse) {
					isTrigger = (IsMouseButtonTriggered(command->m_mouseBtn) || IsKeyTriggered(command->m_xboxKey));
					isRelease = (IsMouseButtonReleased(command->m_mouseBtn) || IsKeyReleased(command->m_xboxKey));
					isPressed = (IsMouseButtonPressed(command->m_mouseBtn) || IsKeyPressed(command->m_xboxKey));
				}
				else if (command->m_isJoystick) {
					isTrigger = (IsKeyTriggered(command->m_keyboardKey) || IsKeyTriggered(command->m_xboxKey));
					isRelease = (IsKeyReleased(command->m_keyboardKey) || IsKeyReleased(command->m_xboxKey));
					isPressed = (IsKeyPressed(command->m_keyboardKey) || IsKeyPressed(command->m_xboxKey));
				}
				else if (command->m_isMouse) {
					isTrigger = IsMouseButtonTriggered(command->m_mouseBtn);
					isRelease = IsMouseButtonReleased(command->m_mouseBtn);
					isPressed = IsMouseButtonPressed(command->m_mouseBtn);
				}
				else {
					isTrigger = IsKeyTriggered(command->m_keyboardKey);
					isRelease = IsKeyReleased(command->m_keyboardKey);
					isPressed = IsKeyPressed(command->m_keyboardKey);
				}
				if (isTrigger || isRelease || isPressed) {
					//INFECT_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputButtonData(isPressed, isTrigger, isRelease)));
				}
				break;
			}
			case InputType_Axis: {
				float xAxis = 0, yAxis = 0;

				if (command->m_isJoystick && command->m_analogue == JoystickAnalogue_Left) {
					if (abs(GetLeftAxisX()) > JoystickDeadZone) xAxis += GetLeftAxisX();
					if (abs(GetLeftAxisY()) > JoystickDeadZone) yAxis -= GetLeftAxisY();
				}
				else if(command->m_isJoystick) {
					if (abs(GetRightAxisX()) > JoystickDeadZone) xAxis += GetRightAxisX();
					if (abs(GetRightAxisY()) > JoystickDeadZone) yAxis -= GetRightAxisX();
				}

				if (IsKeyPressed(command->m_keyboardKeyPosX)) xAxis += 1;
				if (IsKeyPressed(command->m_keyboardKeyNegX)) xAxis -= 1;
				if (IsKeyPressed(command->m_keyboardKeyPosY)) yAxis += 1;
				if (IsKeyPressed(command->m_keyboardKeyNegY)) yAxis -= 1;

				Vector3D axisDir(xAxis, yAxis, 0);
				axisDir.Normalize();
				//TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputAxisData(axisDir)));
				break;
			}
			// This case is unique for our game regarding mouse (Check else statement below)
			case InputType_MousePosOrJoystick: {
				if (command->m_isJoystick && m_isJoystickControlsActive) {
					float xAxis = 0, yAxis = 0;

					if (command->m_analogue == JoystickAnalogue_Left) {
						if (abs(GetLeftAxisX()) > JoystickDeadZone) xAxis += GetLeftAxisX();
						if (abs(GetLeftAxisY()) > JoystickDeadZone) yAxis -= GetLeftAxisY();
					}
					else if (command->m_isJoystick) {
						if (abs(GetRightAxisX()) > JoystickDeadZone) xAxis += GetRightAxisX();
						if (abs(GetRightAxisY()) > JoystickDeadZone) yAxis -= GetRightAxisY();
					}

					Vector3D axisDir(xAxis, yAxis, 0);
					axisDir.Normalize();
					//TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputAxisData(axisDir)));
				}
				else {
					// This is the hardcoded part:
					//if(TETRA_GAME_OBJECTS.GetPlayer())
					//	TETRA_EVENTS.BroadcastEventToSubscribers(&Event(command->m_event, &InputAxisData(Agent::GetDirectionFromPlayerToMouse())));
				}
				break;
			}
		}
	}
}

bool InputManager::IsKeyPressed(const Uint8 scancode) {
	return m_CurrentKeyStates[scancode] & 0x80;
}

bool InputManager::IsKeyTriggered(const Uint8 scancode) {
	return ((m_CurrentKeyStates[scancode] & 0x80) 
			&& !(m_PreviousKeyStates[scancode] & 0x80));
}

bool InputManager::IsKeyReleased(const Uint8 scancode) {
	return (!(m_CurrentKeyStates[scancode] & 0x80) 
		&& (m_PreviousKeyStates[scancode] & 0x80));
}

bool InputManager::IsMouseButtonPressed(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (m_LeftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (m_RightMouse);

	return false;
}

bool InputManager::IsMouseButtonTriggered(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (!m_PrevLeftMouse && m_LeftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (!m_PrevRightMouse && m_RightMouse);

	return false;
}


bool InputManager::IsMouseButtonReleased(MOUSEBTN btn) {
	if (btn == MOUSEBTN::MOUSE_BTN_LEFT)
		return (m_PrevLeftMouse && !m_LeftMouse);
	else if (btn == MOUSEBTN::MOUSE_BTN_RIGHT)
		return (m_PrevRightMouse && !m_RightMouse);

	return false;
}

int InputManager::MousePosX() { return m_MousePosX; }
int InputManager::MousePosY() { return m_MousePosY; }
int InputManager::MousePosRelX() { return m_MousePosRelX; }
int InputManager::MousePosRelY() { return m_MousePosRelY; }


bool InputManager::IsKeyPressed(const XBOX_SCANCODE btn) {
	return m_CurrentButtonStates[btn];
}
bool InputManager::IsKeyTriggered(const XBOX_SCANCODE btn) {
	return (m_CurrentButtonStates[btn] == 1 && m_PreviousButtonStates[btn] == 0);
}
bool InputManager::IsKeyReleased(const XBOX_SCANCODE btn) {
	return (m_CurrentButtonStates[btn] == 0 && m_PreviousButtonStates[btn] == 1);
}

void InputManager::HandleEvent(Event* pEvent) {
	switch (pEvent->Type()) {
		/*case EVENT_INPUT_TOGGLEJOYSTICK: {
			InputButtonData* pButtonData = pEvent->Data<InputButtonData>();
			if (pButtonData->m_isTrigger) m_isJoystickControlsActive = !m_isJoystickControlsActive;
			break;
		}*/
	}
}

Sint16 InputManager::GetLeftAxisX() {
	return m_StickLeftX;
}
Sint16 InputManager::GetLeftAxisY() {
	return m_StickLeftY;
}
Sint16 InputManager::GetRightAxisX() {
	return m_StickRightX;
}
Sint16 InputManager::GetRightAxisY() {
	return m_StickRightY;
}