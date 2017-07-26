#pragma once
#include <D3DX11.h>
#include "Sources\Objects\IRenderable.h"
#include "Sources\Objects\ICharacter.h"

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
	virtual void Move(XMVECTORF32 target) override;
	virtual void Look(XMVECTORF32 direction) override;
	virtual void Attack(void** target) override; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) override;
	virtual void Die() override;

private:

};