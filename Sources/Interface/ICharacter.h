#pragma once

#include <D3DX10math.h>

#include <Sources/Interface/IRenderable.h>

/*
	어차피 ICharacter 인터페이스를 상속받는 클래스 들도
	렌더 대상이기 때문에 IRenderable을 상속시킴
*/

class ICharacter : public IRenderable
{
public:
	virtual void Idle()                      = 0;
	virtual void Move(D3DXVECTOR3 target)    = 0;
	virtual void Attack(void** target)       = 0; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	/*
		편하게 Damage bool값으로 변경 후 데미지 이후에
		바로 메모리제거 처리 가능함
	*/
	virtual void Damage(int damage) = 0;
	virtual void Die()              = 0;

	virtual bool isDied() = 0;
};