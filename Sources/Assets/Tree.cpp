#include <Sources/Assets/Tree.h>

void Tree::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Tree::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);

	Position.x = x;
	Position.y = y;
}

void Tree::Release()
{
	Bitmap.Release();
}

void Tree::Reset()
{
}

void Tree::Update(float dt)
{
}

void Tree::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	Bitmap.Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

int Tree::GetIndexCount()
{
	return Bitmap.GetIndexCount();
}

ID3D10ShaderResourceView * Tree::GetTexture()
{
	return Bitmap.GetTexture();
}

D3DXVECTOR2 Tree::GetPosition() const
{
	return Position;
}

void Tree::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}

void Tree::OnAction(Player * player, float dt)
{
	CoolDown += dt;
	
	if (CoolDown >= 1.5f)
	{
		player->AddItem(ItemIDCanGet[0], 4);
		player->AddItem(ItemIDCanGet[1], 3);
		player->AddItem(ItemIDCanGet[2], 3);
		player->AddItem(ItemIDCanGet[3], 2);

		RefreshCooldown();
	}
}

bool Tree::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == RequiredToolID; });

	return result != player->GetToolList().end();
}

bool Tree::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = Bitmap.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ Position.x - 20, Position.y - 20, Position.x + bitmapSize.width, Position.y + bitmapSize.height }
	);
}

void Tree::RefreshCooldown()
{
	CoolDown = 0.0f;
}