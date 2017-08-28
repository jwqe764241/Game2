#pragma once

#include <vector>
#include <Sources/GameApp.h>

#include <Sources/GameDefs.h>
#include <Sources/GameAssetLoader.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Interface/IRenderable.h>
#include <Sources/Interface/ILevel.h>


class IslandLevel1 : public ILevel {
	// ILevel을(를) 통해 상속됨
	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D10Device * device, int screenWidth, int screenHeight) override;
	virtual void onStart() override;
	virtual void onEnd() override;
};