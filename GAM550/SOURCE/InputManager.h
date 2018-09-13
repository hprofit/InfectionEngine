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
	XBOX_BTN_GUIDE,
	XBOX_BTN_START,
	XBOX_BTN_LEFT_STICK,
	XBOX_BTN_RIGHT_STICK,
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

class InputManager: public Subscriber
{
private:
	// DirectInput
	LPDIRECTINPUT8 DIRX_Interface = nullptr;		// Root DirectInput Interface
	LPDIRECTINPUTDEVICE8 mDIRX_Keyboard = nullptr; // DirectInput keyboard device
	LPDIRECTINPUTDEVICE8 mDIRX_Mouse = nullptr;	// DirectInput mouse device

	// keyboard state
	BYTE *m_PreviousKeyStates;
	BYTE *m_CurrentKeyStates;
	// mouse state
	DIMOUSESTATE m_CurrentMouseStates;
	DIMOUSESTATE m_PreviousMouseStates;

	bool m_PrevLeftMouse, m_LeftMouse;
	bool m_PrevRightMouse, m_RightMouse;
	int m_MousePosX, m_MousePosY;
	int m_MousePosRelX, m_MousePosRelY;
	bool m_isJoystickControlsActive;
	// Game controller States
	BYTE  *m_CurrentButtonStates;
	BYTE  *m_PreviousButtonStates;
	// Axis States
	//Sint16 m_StickRightX, m_StickLeftX;
	//Sint16 m_StickRightY, m_StickLeftY;

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
	bool IsKeyPressed(const XBOX_SCANCODE);
	bool IsKeyTriggered(const XBOX_SCANCODE);
	bool IsKeyReleased(const XBOX_SCANCODE);
	//Sint16 GetLeftAxisX();
	//Sint16 GetLeftAxisY();
	//Sint16 GetRightAxisX();
	//Sint16 GetRightAxisY();
};

#endif