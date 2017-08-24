#pragma once

#include <vector>
#include <Sources/GameApp.h>

#include <Sources/GameDefs.h>
#include <Sources/GameAssetLoader.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Interface/IRenderable.h>
#include <Sources/Interface/ILevel.h>
#include <Sources/Assets/Potal.h>

#include <Sources/Geometries/StateBar.h>

/*
	일단 리스트 2개이고, for문 2개지만 아마 어떻게 해결 할 방법이 있을 것이다 ㅋㅋ...
	맘에 안들긴 함

	렌더 우선순위를 어떻게 관리할 수 있나 생각중이긴 한데 (몇개 있긴 하다)
	근데 이렇게 기능들을 추가해버리면 과연 거기가서 칠 수 있으련지
*/

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

private:
	StateBar stateBar[5];


	EnvironmentList m_EnvironmentList;
	ActorList m_ActorList;


	Player* m_Player;
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
	virtual bool Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight) override;

	virtual void onStart() override;
	virtual void onEnd() override;
};