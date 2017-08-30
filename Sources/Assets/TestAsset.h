#pragma once

#include <d3d10_1.h>

#include <Sources/Geometries/GameSprite.h>
#include <Sources/Shaders/TextureShader.h>
#include <Sources/Interface/IRenderable.h>

class TestAsset : public IRenderable {
private:
	/*
		부쉬로 변경함
		그냥 고정할 예정
	*/
	wchar_t* filePath = L"../Resources/bush.png";

	float m_PosX;
	float m_PosY;

public:
	TestAsset();
	TestAsset(TestAsset& other);
	~TestAsset();

	virtual void Load(ID3D10Device* device,	int bitmapWidth, int bitmapHeight)	override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Render(ID3D10Device* device, int screenWidth, int screenHeight) override;
	virtual int GetIndexCount() override;
	virtual ID3D10ShaderResourceView* GetTexture() override;
	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;

	GameBitmap m_Bitmap;
};