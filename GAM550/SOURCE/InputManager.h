/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Hyoyup Chung
- End Header --------------------------------------------------------*/

#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define MAX_CONTROLLERS 4

enum class MOUSEBTN {
	MOUSE_BTN_LEFT,
	MOUSE_BTN_RIGHT,
	
	MOUSE_NUM_SCANCODES
};
enum XBOX_SCANCODE {
	XBOX_BTN_A,
	XBOX_BTN_B,
	XBOX_BTN_X,
	XBOX_BTN_Y,
	XBOX_BTN_BACK,
	//XBOX_BTN_GUIDE,
	XBOX_BTN_START,
	XBOX_BTN_LEFT_THUMB,
	XBOX_BTN_RIGHT_THUMB,
	XBOX_BTN_LEFT_SHOULDER,
	XBOX_BTN_RIGHT_SHOULDER,
	XBOX_DPAD_UP,
	XBOX_DPAD_DOWN,
	XBOX_DPAD_LEFT,
	XBOX_DPAD_RIGHT,

	XBOX_NUM_SCANCODES
};

enum InputType {
	InputType_Button,
	InputType_Axis,
	InputType_MousePosOrJoystick,

	InputType_NUM
};

enum JoystickAnalogueType {
	JoystickAnalogue_Left,
	JoystickAnalogue_Right,

	JoystickAnalogue_NUM
};

enum PLAYER_NUM {
	PLAYER1,
	PLAYER2,
	PLAYER3,
	PLAYER4,

	PLAYER_SIZE
};

class InputManager: public Subscriber
{
private:
	// DirectInput
	LPDIRECTINPUT8 DIRX_Interface = nullptr;		// DirectInput interface
	LPDIRECTINPUTDEVICE8 mDIRX_Keyboard = nullptr;	// DirectInput keyboard device
	LPDIRECTINPUTDEVICE8 mDIRX_Mouse = nullptr;		// DirectInput mouse device
	// Keyboard State
	BYTE *m_PreviousKeyStates;
	BYTE *m_CurrentKeyStates;
	// Mouse State
	DIMOUSESTATE m_CurrentMouseStates;
	DIMOUSESTATE m_PreviousMouseStates;

	bool m_PrevLeftMouse, m_LeftMouse;
	bool m_PrevRightMouse, m_RightMouse;
	int m_MousePosX, m_MousePosY;
	int m_MousePosRelX, m_MousePosRelY;

	// XInput 
	XINPUT_STATE _xbox_state;
	// Game Controller States
	BYTE  *m_CurrentButtonStates;
	BYTE  *m_PreviousButtonStates;
	// Axis States
	SHORT m_StickRightX, m_StickLeftX;
	SHORT m_StickRightY, m_StickLeftY;

	void FireEvents();
public:
	InputManager();
	~InputManager();
	InputManager(const InputManager &) = delete;
	void operator=(const InputManager &) = delete;
	
	void Init(HINSTANCE hInstance);

	void Update();
	void HandleEvent(Event* pEvent);
	void FreeDirectInput();
	//void Initialize(const json& j);
	// Keyboard Input
	bool IsKeyPressed(const BYTE);
	bool IsKeyTriggered(const BYTE);
	bool IsKeyReleased(const BYTE);
	// Mouse Input
	bool IsMouseButtonPressed(const MOUSEBTN);
	bool IsMouseButtonTriggered(const MOUSEBTN);
	bool IsMouseButtonReleased(const MOUSEBTN);
	int MousePosX(void);
	int MousePosY(void);
	int MousePosRelX(void);
	int MousePosRelY(void);
	// XBOX 360 Controller Input
	bool IsControllerConnected();
	void VibrateController(float leftamt, float rightamt);// Vibration - leftamt, rightamt ranges (0.0f, 1.0f)
	bool IsButtonPressed(const XBOX_SCANCODE);
	bool IsButtonTriggered(const XBOX_SCANCODE);
	bool IsButtonReleased(const XBOX_SCANCODE);
	SHORT GetLeftAxisX();
	SHORT GetLeftAxisY();
	SHORT GetRightAxisX();
	SHORT GetRightAxisY();
};

#endif