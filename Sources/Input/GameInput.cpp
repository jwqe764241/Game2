#include "GameInput.h"

GameInput::GameInput() : m_directInput(nullptr), m_keyboard(nullptr), m_mouse(nullptr)
{
}

GameInput::GameInput(const GameInput &)
{
}

GameInput::~GameInput()
{
}

bool GameInput::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, CLSID_DirectInput8, (void**)&m_directInput, nullptr);
	if (FAILED(result)) {
		return false;
	}

	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr);
	if (FAILED(result)) {
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) {
		return false;
	}

	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) {
		return false;
	}

	result = m_keyboard->Acquire();
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void GameInput::Shutdown()
{
	if (m_mouse) {
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	if (m_keyboard) {
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	if (m_directInput) {
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

bool GameInput::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result) {
		return false;
	}

	result = ReadMouse();
	if (!result) {
		return false;
	}

	ProcessInput();

	return true;
}

bool GameInput::IsEscapeProcessed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80) {
		return true;
	}
	return false;
}

void GameInput::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool GameInput::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result)) {
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			m_keyboard->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}

bool GameInput::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result)) {
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			m_mouse->Acquire();
		}
		else {
			return false;
		}
	}

	return true;
}

void GameInput::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) m_mouseX = 0;
	if (m_mouseY < 0) m_mouseY = 0;

	if (m_mouseX > m_screenWidth) m_mouseX = m_screenWidth;
	if (m_mouseY > m_screenHeight) m_mouseY = m_screenHeight;
}
