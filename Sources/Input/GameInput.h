#pragma once

#include <dinput.h>

class GameInput
{
public:
	GameInput();
	GameInput(const GameInput&);
	~GameInput();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Release();
	bool Frame();

	bool IsEscapeProcessed();
	void GetMouseLocation(int&, int&);

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