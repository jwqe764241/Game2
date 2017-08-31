#pragma once

#include <Sources/Interface/IRenderable.h>
#include <Sources/Assets/Player.h>

class IInteraction : public IRenderable {
	// IRenderable을(를) 통해 상속됨
public:
	virtual void Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y) = 0;
	virtual void Release() override = 0;
	virtual void Reset() override = 0;
	virtual void Update(float dt) override = 0;
	virtual void Render(ID3D10Device * device, int screenWidth, int screenHeight) override = 0;
	virtual int GetIndexCount() override = 0;
	virtual ID3D10ShaderResourceView * GetTexture() override = 0;
	virtual D3DXVECTOR2 GetPosition() const override = 0;
	virtual void SetPosition(const D3DXVECTOR2 pos) override = 0;

	virtual void OnAction(Player * player, float dt) = 0;
	virtual bool CheckTool(Player * player) = 0;
	virtual bool CheckCollision(Player * player) = 0;

	virtual void RefreshCooldown() = 0;
};