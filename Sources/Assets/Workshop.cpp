#include <Sources/Assets/Workshop.h>

void Workshop::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Workshop::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	m_Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);

	m_Position.x = x;
	m_Position.y = y;
}

void Workshop::Release()
{
	m_Bitmap.Release();
}

void Workshop::Reset()
{
}

void Workshop::Update(float dt)
{
}

void Workshop::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	m_Bitmap.Render(device, screenWidth, screenHeight, m_Position.x, m_Position.y);
}

int Workshop::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D10ShaderResourceView * Workshop::GetTexture()
{
	return m_Bitmap.GetTexture();
}

D3DXVECTOR2 Workshop::GetPosition() const
{
	return m_Position;
}

void Workshop::SetPosition(const D3DXVECTOR2 pos)
{
	m_Position = pos;
}

void Workshop::OnAction(Player * player, float dt)
{
	coolDown += dt;

	if (coolDown >= 1.5f)
	{

		RefreshCooldown();
	}
}

bool Workshop::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == m_RequiredToolID; });

	return result != player->GetToolList().end();
}

bool Workshop::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = m_Bitmap.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ m_Position.x - 20, m_Position.y - 20, m_Position.x + bitmapSize.width, m_Position.y + bitmapSize.height }
	);
}

void Workshop::RefreshCooldown()
{
	coolDown = 0.0f;
}
