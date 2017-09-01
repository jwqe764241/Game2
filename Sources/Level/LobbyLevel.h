#pragma once

#include <Sources/GameApp.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Shaders/TextureShader.h>
#include <Sources/GUI/Button.h>
#include <Sources/Interface/ILevel.h>

#include <Sources/Level/DummyLevel.h>
#include <Sources/Level/TestLevel1.h>

class LobbyLevel : public ILevel {
private:
	enum Menu_ID {
		ID_GAMESTART = 0,
		ID_GAMEINTRO,
		ID_GAMEHOWTO,
		ID_GAMERANK,
		ID_GAMESHUTDOWN
	};

	D3DXVECTOR2 button_pos[5] = {
		{ 1520, 50 },
		{ 1520, 250 },
		{ 1520, 450 },
		{ 1520, 650 },
		{ 1520, 850 }
	};

private:
	GameBitmap LevelBitmap;
	CGameCamera Camera;

	Button MenuButtons[5];
	int CurrentSelection;

public:
	LobbyLevel();
	~LobbyLevel();

	// ILevel을(를) 통해 상속됨
	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D10Device * device, int screenWidth, int screenHeight) override;
	virtual void onStart() override;
	virtual void onEnd() override;
};