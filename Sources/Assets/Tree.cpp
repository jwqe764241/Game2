#include <Sources/Assets/Tree.h>

void Tree::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Tree::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	m_Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);

	m_Position.x = x;
	m_Position.y = y;
}

void Tree::Release()
{
	m_Bitmap.Release();
}

void Tree::Reset()
{
}

void Tree::Update(float dt)
{
}

void Tree::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	m_Bitmap.Render(device, screenWidth, screenHeight, m_Position.x, m_Position.y);
}

int Tree::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D10ShaderResourceView * Tree::GetTexture()
{
	return m_Bitmap.GetTexture();
}

D3DXVECTOR2 Tree::GetPosition() const
{
	return m_Position;
}

void Tree::SetPosition(const D3DXVECTOR2 pos)
{
	m_Position = pos;
}

void Tree::OnAction(Player * player, float dt)
{
	coolDown += dt;
	
	if (coolDown >= 1.5f)
	{
		player->AddItem(m_ItemIDCanGet[0], 4);
		player->AddItem(m_ItemIDCanGet[1], 3);
		player->AddItem(m_ItemIDCanGet[2], 3);
		player->AddItem(m_ItemIDCanGet[3], 2);

		RefreshCooldown();
	}
}

bool Tree::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == m_RequiredToolID; });

	return result != player->GetToolList().end();
}

bool Tree::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = m_Bitmap.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ m_Position.x - 20, m_Position.y - 20, m_Position.x + bitmapSize.width, m_Position.y + bitmapSize.height }
	);
}

void Tree::RefreshCooldown()
{
	coolDown = 0.0f;
}