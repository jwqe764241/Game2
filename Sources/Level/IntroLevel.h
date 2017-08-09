#pragma once

#include <Sources/Interface/ILevel.h>
#include <Sources/Camera/GameCamera.h>

class IntroLevel : public ILevel{
private:
	CGameCamera m_Camera;
	
public:
	IntroLevel();
	~IntroLevel();

	// ILevel을(를) 통해 상속됨
	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight) override;
	virtual void onStart() override;
	virtual void onEnd() override;
};