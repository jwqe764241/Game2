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
#include <Sources/Assets/Tool.h>

#include <Sources/Assets/Rock.h>
#include <Sources/Assets/Pond.h>
#include <Sources/Assets/Tree.h>
#include <Sources/Assets/Deer.h>

#include <Sources/Level/GameOverLevel.h>


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
	
	const environmentElement environmentList[8] = {
		{ L"../Resources/Rock1.png", 171, 97, 1600, 100},
		{ L"../Resources/Rock2.png", 171, 125, 1700, 200},
		{ L"../Resources/Rock3.png", 165, 121, 1700, 300},
		{ L"../Resources/pond.png", 788, 277, 6300, 100 },
		{ L"../Resources/Tree.png", 1000, 888, 6300, 1000 },
		{ L"../Resources/Tree.png", 1000, 888, 6000, 825 },
		{ L"../Resources/Tree.png", 1000, 888, 5200, 1200 },
		{ L"../Resources/Tree.png", 1000, 888, 5800, 1500 },
	};

	const POINT deerPosition[5] = {
		{4000, 2000},
		{3900, 1750},
		{4200, 1900},
		{4150, 2150},
		{4450, 2000}
	};

private:
	std::vector<Tool *> ToolList;
	std::vector<ICharacter *> ActorList;
	std::vector<IInteraction *> EnvironmentList;
	std::vector<Deer *> DeerList;

	GameBitmap LevelBitmap;
	GameBitmap Cursor;
	CGameCamera Camera;

	RECT LevelSize;

	Player* GamePlayer;

	StateUI PlayerUI;

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