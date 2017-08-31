#pragma once

#include <Sources/Interface/IInteraction.h>
#include <Sources/Geometries/GameBitmap.h>

class Workshop : public IInteraction {
private:
	GameBitmap m_Bitmap;
	D3DXVECTOR2 m_Position;

public:
	// IInteraction��(��) ���� ��ӵ�
	virtual void Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Render(ID3D10Device * device, int screenWidth, int screenHeight) override;
	virtual int GetIndexCount() override;
	virtual ID3D10ShaderResourceView * GetTexture() override;
	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
	virtual void OnAction(Player * player, float dt) override;
	virtual bool CheckTool(Player * player) override;
	virtual bool CheckCollision(Player * player) override;
	virtual void RefreshCooldown() override;
};