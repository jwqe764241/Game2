#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

#include <Sources/Interface/IRenderable.h>
#include <Sources/Geometries/GameSprite.h>

class SpriteAsset : public IRenderable
{
	wchar_t* m_ResourcePath = L"../Resources/sprite.bmp";

	GameSprite m_Object;

	float m_PosX, m_PosY;

public:
	SpriteAsset();
	~SpriteAsset();

	virtual void Load(ID3D11Device* device, int bitmapWidth, int bitmapHeight)	override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight) override;
	virtual int GetIndexCount() override;
	virtual ID3D11ShaderResourceView* GetTexture() override;
	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
	virtual GameSprite* GetSprite() override;
};