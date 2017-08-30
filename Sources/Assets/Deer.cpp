#include <Sources/Assets/Deer.h>

Deer::Deer() : m_Sprite(15.0f, 1.0f)
{

}

void Deer::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Deer::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	m_Sprite.Initialize(device, filePath, bitmapWidth, bitmapHeight, 4);

	m_Position.x = x;
	m_Position.y = y;
}

void Deer::Release()
{
	m_Sprite.Release();
}

void Deer::Reset()
{
}

void Deer::Update(float dt)
{
	static int accrueX = 0;

	float x = 0.0f;
	float y = 0.0f;
	float speed = 250.0f;

	if (accrueX <= 450)
	{
		m_Sprite.SetMotion(2);
		x += 1;
	}
	else if (accrueX > 450)
	{
		m_Sprite.SetMotion(1);
		x -= 1;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0)
	{
		speed *= dt;
		x *= speed / length;

		m_Position.x += x;

		accrueX += x;
	}
}

void Deer::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	m_Sprite.Render(device, screenWidth, screenHeight, m_Position.x, m_Position.y);
}

int Deer::GetIndexCount()
{
	return m_Sprite.GetIndexCount();
}

ID3D10ShaderResourceView * Deer::GetTexture()
{
	return m_Sprite.GetTexture();
}

D3DXVECTOR2 Deer::GetPosition() const
{
	return m_Position;
}

void Deer::SetPosition(const D3DXVECTOR2 pos)
{
	m_Position = pos;
}

void Deer::OnAction(Player * player, float dt)
{
	coolDown += dt;

	if (coolDown >= 0.5f)
	{
		m_Health -= 1;
		
		RefreshCooldown();
	}
}

bool Deer::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == m_RequiredToolID; });

	return result != player->GetToolList().end();
}

bool Deer::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = m_Sprite.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ m_Position.x - 20, m_Position.y - 20, m_Position.x + bitmapSize.width, m_Position.y + bitmapSize.height }
	);
}

void Deer::RefreshCooldown()
{
	coolDown = 0.0f;
}

bool Deer::IsDead()
{
	return m_Health <= 0;
}
