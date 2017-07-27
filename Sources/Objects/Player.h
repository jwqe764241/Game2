#pragma once

#include <D3DX10math.h>
#include "Sources\Objects\IRenderable.h"
#include "Sources\Objects\ICharacter.h"
#include "..\Geometries\GameBitmap.h"

class Player : public IRenderable, public ICharacter
{
public:
	Player();
	~Player();

	virtual void Load() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
			void Update(float dt, int keyCode);
	virtual void Render() override;

	virtual void Idle() override;
	virtual void Move(D3DXVECTOR3 target) override;
	virtual void Attack(void** target) override; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) override;
	virtual void Die() override;

	bool IsDied();
private:
	D3DXVECTOR3 m_Position;

	int m_Health;

	bool m_IsDied;

	GameBitmap* m_Object;
};