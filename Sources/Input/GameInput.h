#pragma once

#include <dinput.h>

#include <Sources/GameDefs.h>

enum MouseCode {
	LBUTTONLICKED = 0,
	RBUTTONCLICKED,
	WHEELBUTTONCLICKED
};

class GameInput
{
public:
	GameInput();
	GameInput(const GameInput&);
	~GameInput();

	bool Initialize(HINSTANCE, HWND, int, int);
	static GameInput& GetInstance();
	void Release();
	bool Frame();

	bool IsEscapeProcessed();
	bool IsKeyPressed(DWORD keycode);
	bool IsMousePressed(DWORD keycode);

	/*
		사용하지 말것 GetCursorPos()를 사용할 것
	*/
	POINT GetMousePosition();

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};