#include <Sources/Assets/Enemy1.h>

Enemy1::Enemy1() : Sprite(15.0f, 1.0f, true), ResourcePath(L"../Resources/sprite.bmp")
{

}

Enemy1::~Enemy1()
{

}

void Enemy1::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
	Position.x = 0;
	Position.y = 0;
	Health = 100;
	Sprite.Initialize(device, ResourcePath, bitmapWidth, bitmapHeight, 4);
	//Sprite.SetLooping(false);
	Sprite.SetMotion(2);
}

void Enemy1::Release()
{
	Sprite.Release();
}

void Enemy1::Reset()
{

}

void Enemy1::Update(float dt)
{
	Sprite.Update(dt);
}

void Enemy1::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	Sprite.Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

int Enemy1::GetIndexCount()
{
	return Sprite.GetIndexCount();
}

ID3D10ShaderResourceView * Enemy1::GetTexture()
{
	return Sprite.GetTexture();
}

D3DXVECTOR2 Enemy1::GetPosition() const
{
	return Position;
}

void Enemy1::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}

GameSprite * Enemy1::GetSprite()
{
	return &Sprite;
}


void Enemy1::Idle()
{
	
}

void Enemy1::Move(D3DXVECTOR2 target)
{
	Position.x = target.x;
	Position.y = target.y;
}

void Enemy1::Attack(void ** target)
{

}

void Enemy1::Damage(int damage)
{
	if (Health - damage <= 0)
	{
		Health = 0;
	}
	else
	{
		Health -= damage;
	}
}

void Enemy1::Die()
{
	Health = 0;
}

bool Enemy1::isDied()
{
	if (Health <= 0)
	{
		return true;
	}

	return false;
}
