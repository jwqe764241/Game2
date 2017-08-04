#pragma once

#include <D3D11.h>

#include <Sources/Geometries/GameSprite.h>
#include <Sources/Interface/IRenderable.h>
#include <Sources/Interface/ICharacter.h>

class Enemy1 : public ICharacter{
private:
	wchar_t* m_ResourcePath;

	int m_Health;
	D3DXVECTOR2 m_Pos;
	GameSprite m_Sprite;

public:
	Enemy1();
	~Enemy1();

	// IRenderable을(를) 통해 상속됨
	virtual void Load(ID3D11Device * device, int bitmapWidth, int bitmapHeight) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight) override;
	virtual int GetIndexCount() override;
	virtual ID3D11ShaderResourceView * GetTexture() override;
	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
	virtual GameSprite * GetSprite() override;

	// ICharacter을(를) 통해 상속됨
	virtual void Idle() override;
	virtual void Move(D3DXVECTOR2 target) override;
	virtual void Attack(void ** target) override;
	virtual void Damage(int damage) override;
	virtual void Die() override;
	virtual bool isDied() override;

};