#pragma once

#include <Sources/Interface/ILevel.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Geometries/GameBitmap.h>
#include <Sources/GameTextWriter.h>
#include <Sources/GUI/Button.h>
#include <Sources/GameApp.h>

class GameOverLevel : public ILevel {
private:
	GameBitmap LevelBitmap;
	CGameCamera Camera;

	Button MenuButtons[2];
	int CurrentSelection;

	GameTextWriter writer_48size;

public:
	// ILevel을(를) 통해 상속됨
	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D10Device * device, int screenWidth, int screenHeight) override;
	virtual void onStart() override;
	virtual void onEnd() override;
};