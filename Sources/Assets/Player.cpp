#include <Sources/Assets/Player.h>

Player::Player() : m_Sprite(15.0f, 1.0f, true), m_ResourcePath(L"../Resources/sprite.bmp")
{

}

Player::~Player()
{

}

void Player::Load(ID3D11Device * device, int bitmapWidth, int bitmapHeight)
{
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Health = 100;
	m_Sprite.Initialize(device, m_ResourcePath, bitmapWidth, bitmapHeight, 4);
	m_Sprite.SetLooping(false);
	m_Sprite.SetMotion(2);
}

void Player::Release()
{
	m_Sprite.Release();
}

void Player::Reset()
{

}

void Player::Update(float dt)
{
	GameInput& input = GameInput::GetInstance();
	float x = 0.0f, y = 0.0f;
	float speed = 150.0f;

	if (input.IsPressed(DIK_W)) 
	{
		m_Sprite.SetMotion(0);
		y -= 1;
	}
	else if (input.IsPressed(DIK_A))
	{
		m_Sprite.SetMotion(1);
		x -= 1;
	}
	else if (input.IsPressed(DIK_S))
	{
		m_Sprite.SetMotion(2);
		y += 1;
	}
	else if (input.IsPressed(DIK_D))
	{
		m_Sprite.SetMotion(3);
		x += 1;
	}
	else
	{
		m_Sprite.SetLooping(false);
		m_Sprite.Update(dt);
		return;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0) {
		speed *= dt;
		x *= speed / length;
		y *= speed / length;
		m_Pos.x += x;
		m_Pos.y += y;
	}

	m_Sprite.SetLooping(true);
	m_Sprite.Update(dt);
}

void Player::Update(float dt, int keyCode)
{

}

void Player::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{
	m_Sprite.Render(deviceContext, screenWidth, screenHeight, m_Pos.x, m_Pos.y);
}

void Player::Idle()
{
	m_Sprite.SetLooping(false);
}

//어디다가 쓰지..
void Player::Move(D3DXVECTOR3 target)
{
	m_Pos.x = target.x;
	m_Pos.y = target.y;
}

void Player::Attack(void ** target)
{

}

void Player::Damage(int damage)
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

void Player::Die()
{
	m_Health = 0;
}

bool Player::isDied()
{
	if (m_Health <= 0)
	{
		return false;
	}
	return false;
}

int Player::GetIndexCount()
{
	return m_Sprite.GetIndexCount();
}

ID3D11ShaderResourceView* Player::GetTexture()
{
	return m_Sprite.GetTexture();
}

D3DXVECTOR2 Player::GetPosition() const
{
	return m_Pos;
}

void Player::SetPosition(const D3DXVECTOR2 pos)
{
	m_Pos = pos;
}

GameSprite* Player::GetSprite()
{
	return &m_Sprite;
}