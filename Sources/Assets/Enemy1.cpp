#include <Sources/Assets/Enemy1.h>

Enemy1::Enemy1() : m_Sprite(15.0f, 1.0f, true), m_ResourcePath(L"../Resources/sprite.bmp")
{

}

Enemy1::~Enemy1()
{

}

void Enemy1::Load(ID3D11Device * device, int bitmapWidth, int bitmapHeight)
{
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Health = 100;
	m_Sprite.Initialize(device, m_ResourcePath, bitmapWidth, bitmapHeight, 4);
	//m_Sprite.SetLooping(false);
	m_Sprite.SetMotion(2);
}

void Enemy1::Release()
{
	m_Sprite.Release();
}

void Enemy1::Reset()
{

}

void Enemy1::Update(float dt)
{
	m_Sprite.Update(dt);
}

void Enemy1::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{
	m_Sprite.Render(deviceContext, screenWidth, screenHeight, m_Pos.x, m_Pos.y);
}

int Enemy1::GetIndexCount()
{
	return m_Sprite.GetIndexCount();
}

ID3D11ShaderResourceView * Enemy1::GetTexture()
{
	return m_Sprite.GetTexture();
}

D3DXVECTOR2 Enemy1::GetPosition() const
{
	return m_Pos;
}

void Enemy1::SetPosition(const D3DXVECTOR2 pos)
{
	m_Pos = pos;
}

GameSprite * Enemy1::GetSprite()
{
	return &m_Sprite;
}


void Enemy1::Idle()
{
	
}

void Enemy1::Move(D3DXVECTOR3 target)
{
	m_Pos.x = target.x;
	m_Pos.y = target.y;
}

void Enemy1::Attack(void ** target)
{

}

void Enemy1::Damage(int damage)
{
	if (m_Health - damage <= 0)
	{
		m_Health = 0;
	}
	else
	{
		m_Health -= damage;
	}
}

void Enemy1::Die()
{
	m_Health = 0;
}

bool Enemy1::isDied()
{
	if (m_Health <= 0)
	{
		return true;
	}

	return false;
}
