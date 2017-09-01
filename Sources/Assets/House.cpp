#include <Sources/Assets/House.h>

void House::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void House::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);
	
	Position.x = x;
	Position.y = y;
}

void House::Release()
{
	Bitmap.Release();
}

void House::Reset()
{
}

void House::Update(float dt)
{
}

void House::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	Bitmap.Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

int House::GetIndexCount()
{
	return Bitmap.GetIndexCount();
}

ID3D10ShaderResourceView * House::GetTexture()
{
	return Bitmap.GetTexture();
}

D3DXVECTOR2 House::GetPosition() const
{
	return Position;
}

void House::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}

void House::OnAction(Player * player, float dt)
{
	CoolDown += dt;

	if (CoolDown >= 1.5f)
	{
		player->SetSleepValue(player->GetSleepValue() + 10);
		RefreshCooldown();
	}
}

bool House::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == RequiredToolID; });

	return result != player->GetToolList().end();
}

bool House::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = Bitmap.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ Position.x - 20, Position.y - 20, Position.x + bitmapSize.width, Position.y + bitmapSize.height }
	);
}

void House::RefreshCooldown()
{
	CoolDown = 0.0f;
}
