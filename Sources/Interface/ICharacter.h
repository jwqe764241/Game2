#pragma once

#include <Windows.h>
#include <xnamath.h>

class ICharacter
{
public:
	virtual void Idle()                      = 0;
	virtual void Move(XMVECTORF32 target)    = 0;
	virtual void Look(XMVECTORF32 direction) = 0;
	virtual void Attack(void** target)       = 0; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) = 0;
	virtual void Die()              = 0;

	virtual bool isDied() = 0;
};