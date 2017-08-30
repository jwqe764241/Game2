#pragma once

#include <vector>
#include <algorithm>
#include <Sources/GameApp.h>
#include <Sources/GameDefs.h>
#include <Sources/GameAssetLoader.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Assets/StateUI.h>
#include <Sources/Interface/ILevel.h>
#include <Sources/Assets/Rock.h>
#include <Sources/Assets/Tool.h>

/*
	일단 리스트 2개이고, for문 2개지만 아마 어떻게 해결 할 방법이 있을 것이다 ㅋㅋ...
	맘에 안들긴 함

	렌더 우선순위를 어떻게 관리할 수 있나 생각중이긴 한데 (몇개 있긴 하다)
	근데 이렇게 기능들을 추가해버리면 과연 거기가서 칠 수 있으련지
*/

struct environmentElement
{
	wchar_t * filePath;
	float width;
	float height;
	int x;
	int y;
};

class TestLevel1 : public ILevel {
private:
	const int cameraPosX[4] = {
		0, 1920, 3840, 5760
	};

	const int cameraPosY[4] = {
		0, -1080, -2160, -3240
	};

	const POINT toolsPosition[5] = {
		{3500, 2500},
		{1500, 100},
		{7300, 4000},
		{300, 4000},
		{6300, 1000}
	};
	
	const environmentElement environmentList[3] = {
		{ L"../Resources/Rock1.png", 171, 97, 1600, 100},
		{ L"../Resources/Rock2.png", 171, 125, 1700, 200},
		{ L"../Resources/Rock3.png", 165, 121, 1700, 300}
	};

private:
	std::vector<Tool *> m_Tools;
	std::vector<ICharacter *> m_ActorList;
	std::vector<IInteraction *> m_EnvironmentList;
	
	Player* m_Player;

	StateUI m_PlayerUI;

	CGameCamera m_Camera;

	GameBitmap m_Cursor;

	RECT m_LevelSize;
	GameBitmap m_LevelBitmap;


public:
	TestLevel1();
	~TestLevel1();

	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D10Device* device, int screenWidth, int screenHeight) override;

	virtual void onStart() override;
	virtual void onEnd() override;
	void onGameOver();
};