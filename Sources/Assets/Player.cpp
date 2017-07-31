#include <Sources/Assets/Player.h>

Player::Player() : m_Sprite(15.0f, 1.0f, true)
{

}

Player::~Player()
{

}

void Player::Load(ID3D11Device * device, int bitmapWidth, int bitmapHeight)
{
	m_PosX = 0;
	m_PosY = 0;
	m_Speed = 1.0f;
	m_Sprite.Initialize(device, m_ResourcePath, bitmapWidth, bitmapHeight, 4);
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
	m_Sprite.Update(dt);

	GameInput& input = GameInput::GetInstance();
	float x = 0.0f, y = 0.0f;
	float speed = 50.0f;

	//TODO:µ¨Å¸Å¸ÀÓ Àû¿ë ¾ÈµÊ
	if (input.IsPressed(DIK_W)) 
	{
		m_Sprite.SetMotion(0);
		y -= 5;
	}
	else if (input.IsPressed(DIK_A))
	{
		m_Sprite.SetMotion(1);
		x -= 5;
	}
	else if (input.IsPressed(DIK_S))
	{
		m_Sprite.SetMotion(2);
		y += 5;
	}
	else if (input.IsPressed(DIK_D))
	{
		m_Sprite.SetMotion(3);
		x += 5;
	}
	else
	{
		Idle();
		m_Sprite.Update(dt);
		return;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0) {
		speed *= dt;
		x *= speed / length;
		y *= speed / length;
		m_PosX += x;
		m_PosY += y;
	}
}

void Player::Update(float dt, int keyCode)
{

}

void Player::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{
	m_Sprite.Render(deviceContext, screenWidth, screenHeight, m_PosX, m_PosY);
}

void Player::Idle()
{
	m_Sprite.SetMotion(2);
}

void Player::Move(D3DXVECTOR3 target)
{

}

void Player::Attack(void ** target)
{

}

void Player::Damage(int damage)
{

}

void Player::Die()
{

}

bool Player::isDied()
{

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