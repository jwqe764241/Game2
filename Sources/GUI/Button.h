#pragma once

#include <d3d10_1.h>

#include <Sources/Geometries/GameBitmap.h>
#include <Sources/Interface/IRenderable.h>

class Button : public IRenderable{
private:
	GameBitmap* m_statusBitmap[2];
	GameBitmap* m_pCurrBitmap = nullptr;

	D3DXVECTOR2 m_pos;

public:
	Button();
	~Button();

	// IRenderable을(를) 통해 상속됨
	virtual void Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
			void Update(bool selected);
	virtual void Render(ID3D10Device * device, int screenWidth, int screenHeight) override;
	virtual int GetIndexCount() override;
	virtual ID3D10ShaderResourceView * GetTexture() override;
	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
};