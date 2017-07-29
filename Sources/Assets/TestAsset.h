#ifndef __TESTASSET_H
#define __TESTASSET_H

#include <D3D11.h>
#include <D3DX11.h>

#include <Sources/Geometries/GameBitmap.h>
#include <Sources/Shaders/TextureShader.h>
#include <Sources/Objects/IRenderable.h>

class TestAsset : public IRenderable {
private:
	wchar_t* filePath = L"../Resources/test.bmp";

	float m_posX;
	float m_posY;

public:
	TestAsset();
	TestAsset(TestAsset& other);
	~TestAsset();

	virtual void Load(ID3D11Device* device, int screenWidth, int screenHeight, 
		int bitmapWidth, int bitmapHeight)	override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Render(ID3D11DeviceContext* deviceContext) override;
	virtual int GetIndexCount() override;
	virtual ID3D11ShaderResourceView* GetTexture() override;

	GameBitmap m_Bitmap;
};

#endif