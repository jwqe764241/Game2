#pragma once
#include <d3d11.h>
#include <D3DX10math.h>
#include "Sources\Objects\IRenderable.h"
#include "Sources\Objects\ICharacter.h"
#include "Sources\Geometries\GameBitmap.h"

class Player : public IRenderable, public ICharacter
{
public:
	Player();
	~Player();

	virtual void Load(ID3D11Device*, int, int) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
			void Update(float dt, int keyCode);
	virtual void Render(ID3D11DeviceContext*, int, int) override;

	virtual void Idle() override;
	virtual void Move(D3DXVECTOR3 target) override;
	virtual void Attack(void** target) override; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) override;
	virtual void Die() override;

	bool isDied() { return isDied; };

protected:
	int m_Health;
	bool m_IsDied;

	GameBitmap* m_Object;
};