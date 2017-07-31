#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

class ICharacter
{
public:
	virtual void Idle()                      = 0;
	virtual void Move(D3DXVECTOR3 target)    = 0;
	virtual void Attack(void** target)       = 0; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) = 0;
	virtual void Die()              = 0;
};