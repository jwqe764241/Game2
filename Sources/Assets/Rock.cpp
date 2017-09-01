#include <Sources/Assets/Rock.h>

void Rock::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Rock::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);

	Position.x = x;
	Position.y = y;
}


void Rock::Release()
{
	Bitmap.Release();
}

void Rock::Reset()
{
}

void Rock::Update(float dt)
{
}

void Rock::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	Bitmap.Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

int Rock::GetIndexCount()
{
	return Bitmap.GetIndexCount();
}

ID3D10ShaderResourceView * Rock::GetTexture()
{
	return Bitmap.GetTexture();
}

D3DXVECTOR2 Rock::GetPosition() const
{
	return Position;
}

void Rock::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}

void Rock::OnAction(Player * player, float dt)
{
	CoolDown += dt;

	if (CoolDown >= 1.0f)
	{
		player->AddItem(ItemIDCanGet, 2);
		RefreshCooldown();
	}
}

bool Rock::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == RequiredToolID; });

	return result != player->GetToolList().end();
}

bool Rock::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = Bitmap.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ Position.x - 20, Position.y - 20, Position.x + bitmapSize.width, Position.y + bitmapSize.height }
	);
}

void Rock::RefreshCooldown()
{
	CoolDown = 0.0f;
}