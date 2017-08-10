#pragma once

#include <Sources/GameApp.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Geometries/GameSprite.h>
#include <Sources/GUI/Button.h>
#include <Sources/Interface/ILevel.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Level/GameLevelLoader.h>
#include <Sources/Level/LobbyLevel.h>\

class DummyLevel : public ILevel{
private:
	CGameCamera m_Camera;
	GameBitmap m_BackgroundBitmap;
	Button m_BackButton;
public:
	DummyLevel();
	~DummyLevel();

	// ILevel을(를) 통해 상속됨
	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight) override;
	virtual void onStart() override;
	virtual void onEnd() override;
};