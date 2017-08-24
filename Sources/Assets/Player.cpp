#include <Sources/Assets/Player.h>

static float waterCoolDown = 0.0f;
static float foodCoolDown = 0.0f;
static float sleepCoolDown = 0.0f;

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
	isSetPositionLimit = false;
	
	m_WaterValue = 100;
	m_FoodValue = 100;
	m_SleepValue = 100;
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
	waterCoolDown += dt;
	foodCoolDown += dt;
	sleepCoolDown += dt;

	if (waterCoolDown >= 1.0f)
	{
		m_WaterValue -= 1;
		waterCoolDown = 0.0f;
	}
	if (foodCoolDown >= 2.0f)
	{
		m_FoodValue -= 1;
		foodCoolDown = 0.0f;
	}
	if (sleepCoolDown >= 3.0f)
	{
		m_SleepValue -= 1;
		sleepCoolDown = 0.0f;
	}

	GameInput2& input = GameInput2::GetInstance();
	float x = 0.0f, y = 0.0f;
	float speed = 350.0f;

	if (input.IsPressed(VK_RIGHT))
	{
		x += 1.0f;
		m_Sprite.SetMotion(3);
	}
	else if (input.IsPressed(VK_LEFT))
	{
		x -= 1.0f;
		m_Sprite.SetMotion(1);
	}
	else if (input.IsPressed(VK_UP))
	{
		y -= 1.0f;
		m_Sprite.SetMotion(0);
	}
	else if (input.IsPressed(VK_DOWN))
	{
		y += 1.0f;
		m_Sprite.SetMotion(2);
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
		if (isSetPositionLimit)
		{
			if (m_Pos.x + x >= m_PositionLimit.left && (m_Pos.x + x) + m_Sprite.GetFrameWidth() <= m_PositionLimit.right)
			{
				m_Pos.x += x;
			}

			if (m_Pos.y + y >= m_PositionLimit.top && (m_Pos.y + y) + m_Sprite.GetFrameHeight() <= m_PositionLimit.bottom)
			{
				m_Pos.y += y;
			}
		}
		else
		{
			m_Pos.x += x;
			m_Pos.y += y;
		}
	}

	m_Sprite.SetLooping(true);
	m_Sprite.Update(dt);
}

void Player::Update(float dt, CGameCamera* pCamera)
//해당 업데이트는 마우스 클릭 시 해당 마우스 좌표로 이동하게 하는 시뮬레이션 함수임
{
	static bool isWalking = false;
	static POINT targetPoint{ 0.0f, 0.0f };

	float x = 0.0f, y = 0.0f;
	float speed = 350.0f;

	if (GameInput2::GetInstance().IsPressed(VK_LBUTTON))
	{
		POINT mousePos = GameInput2::GetInstance().GetMousePosition();
		D3DXVECTOR3 cameraPos = pCamera->GetPosition();
		//Move(D3DXVECTOR2{ po.x + cameraPos.x, po.y - cameraPos.y});
		targetPoint.x = mousePos.x + cameraPos.x - 32;
		targetPoint.y = mousePos.y - cameraPos.y - 32;
		isWalking = true;
	}
	
	if (!isWalking)
	{
		return;
	}

	if (m_Pos.x < targetPoint.x)
	{
		x += 1.0f;
	}
	else
	{
		x -= 1.0f;
	}

	if (m_Pos.y < targetPoint.y)
	{
		y += 1.0f;
	}
	else
	{
		y -= 1.0f;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0) {

		if ((m_Pos.x >= (targetPoint.x - 1.0f)) && (m_Pos.x <= (targetPoint.x + 1.0f)))
		{
			if ((m_Pos.y >= (targetPoint.y - 1.0f)) && (m_Pos.y <= (targetPoint.y + 1.0f)))
			{
				isWalking = false;
				m_Sprite.SetLooping(false);
				return;
			}
		}

		speed *= dt;
		x *= speed / length;
		y *= speed / length;

		if (isSetPositionLimit)
		{
			if (m_Pos.x >= m_PositionLimit.left && m_Pos.x + m_Sprite.GetFrameWidth() / 2 <= m_PositionLimit.right)
			{
				m_Pos.x += x;
			}

			if (m_Pos.y >= m_PositionLimit.top && m_Pos.y + m_Sprite.GetFrameHeight() / 2 <= m_PositionLimit.bottom)
			{
				m_Pos.y += y;
			}
		}
		else
		{
			m_Pos.x += x;
			m_Pos.y += y;
		}

	}

	m_Sprite.SetLooping(true);
	m_Sprite.Update(dt);
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
void Player::Move(D3DXVECTOR2 target)
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

void Player::SetPositionLimit(const RECT* limitPos)
{
	if (limitPos == nullptr)
	{
		isSetPositionLimit = false;
		m_PositionLimit = { 0 };
	}
	else
	{
		isSetPositionLimit = true;
		m_PositionLimit = (*limitPos);
	}
}

int Player::GetHealth() const
{
	return m_Health;
}

int Player::GetWaterValue() const
{
	return m_WaterValue;
}

int Player::GetFoodValue() const
{
	return m_FoodValue;
}

int Player::GetSleepValue() const
{
	return m_SleepValue;
}