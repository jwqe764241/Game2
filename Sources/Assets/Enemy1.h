#pragma once

#include <d3d10_1.h>

#include <Sources/Geometries/GameSprite.h>
#include <Sources/Interface/ICharacter.h>

class Enemy1 : public ICharacter{
private:
	wchar_t* ResourcePath;

	int Health;
	D3DXVECTOR2 Position;
	GameSprite Sprite;

public:
	Enemy1();
	~Enemy1();

	// IRenderable을(를) 통해 상속됨
	virtual void Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Render(ID3D10Device * device, int screenWidth, int screenHeight) override;
	virtual int GetIndexCount() override;
	virtual ID3D10ShaderResourceView * GetTexture() override;
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