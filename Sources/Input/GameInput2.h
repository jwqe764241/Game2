#pragma once

#include <Windows.h>
#include <cassert>

/*
	GameInput의 다른 구현임
	이전 GameInput의 문제는 다음과 같다.
	 1. Direct Input 을 사용하면 메시지를 받기 위해 쓰레드가 내부에서 생성된다.
	 2. 마우스 움직임 누적값의 오차가 있다.
	 3.마우스 좌표는 절대 좌표 누적값이다.

	그렇기 때문에 해당 윈도우 기준 좌표값을 구하기에 많은 제약사항과 요류가 있다.
	공용자원으로 놓고 프로시저에서 값을 업데이트 하면서 입력값을 얻는다.
*/

class GameInput2 {
private:
	unsigned char m_KeyboardState[256];
	POINT m_MousePosition;

public:
	GameInput2();
	~GameInput2();
	
	static GameInput2& GetInstance();

	/*
		초기화 시점에서 계산을 할 것이기 때문에
		해당 윈도우 핸들을 넘겨받는다.
	*/
	void Initialize(const HWND hWnd);

	/*
		해당 함수 public으로 결정된 것은 아님
		가능한 숨겨야 함
	*/
	void PressKey(int keycode);
	void ReleaseKey(int keycode);
	void UpdateMousePosition(POINT pos);

	bool IsPressed(int keycode) const;
	POINT GetMousePosition() const;
};