#pragma once

#include <dinput.h>

#include <Sources/GameDefs.h>

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1

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
	bool IsPressed(DWORD keycode);

	bool IsMousePressed(int);
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