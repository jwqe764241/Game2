#include <Sources/Input/GameInput2.h>

GameInput2::GameInput2()
{
}

GameInput2::~GameInput2()
{

}

GameInput2& GameInput2::GetInstance()
{
	static GameInput2 instance;

	return instance;
}

void GameInput2::Initialize(const HWND hWnd)
{
	assert(hWnd != 0 && "hWnd cannot be 0 value");

	memset(&m_KeyboardState, 0x00, sizeof(unsigned char) * 256);
	memset(&m_MousePosition, 0x00, sizeof(POINT));
}

void GameInput2::PressKey(int keycode)
{
	assert(keycode >= 0 && keycode <= 255 && "keycode bound error!!");
	/*
		이것만으로 세이프 할까..
	*/
	m_KeyboardState[keycode] = 0x80;
}

void GameInput2::ReleaseKey(int keycode)
{
	assert(keycode >= 0 && keycode <= 255 && "keycode bound error!!");

	m_KeyboardState[keycode] = 0x00;
}


void GameInput2::UpdateMousePosition(POINT pos)
{
	m_MousePosition.x = pos.x;
	m_MousePosition.y = pos.y;
}

bool GameInput2::IsPressed(int keycode) const
{
	return m_KeyboardState[keycode] & 0x80;
}

bool GameInput2::IsReleased(int keycode) const
{
	return !(m_KeyboardState[keycode] & 0x80);
}

POINT GameInput2::GetMousePosition() const
{
	return m_MousePosition;
}