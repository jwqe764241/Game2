#include <Sources/Assets/Deer.h>

Deer::Deer() : Sprite(15.0f, 1.0f)
{

}

void Deer::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Deer::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	Sprite.Initialize(device, filePath, bitmapWidth, bitmapHeight, 4);

	Position.x = x;
	Position.y = y;
}

void Deer::Release()
{
	Sprite.Release();
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
		Sprite.SetMotion(2);
		x += 1;
	}
	else if (accrueX > 450)
	{
		Sprite.SetMotion(1);
		x -= 1;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0)
	{
		speed *= dt;
		x *= speed / length;

		Position.x += x;

		accrueX += x;
	}

	Sprite.SetLooping(true);
	Sprite.Update(dt);
}

void Deer::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	Sprite.Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

int Deer::GetIndexCount()
{
	return Sprite.GetIndexCount();
}

ID3D10ShaderResourceView * Deer::GetTexture()
{
	return Sprite.GetTexture();
}

D3DXVECTOR2 Deer::GetPosition() const
{
	return Position;
}

void Deer::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}

void Deer::OnAction(Player * player, float dt)
{
	CoolDown += dt;

	if (CoolDown >= 0.5f)
	{
		Health -= 1;
		
		RefreshCooldown();
	}
}

bool Deer::CheckTool(Player * player)
{
	auto result = std::find_if(player->GetToolList().begin(), player->GetToolList().end(),
		[this](const Tool* p) { return p->GetToolID() == RequiredToolID; });

	return result != player->GetToolList().end();
}

bool Deer::CheckCollision(Player * player)
{
	BitmapSize bitmapSize = Sprite.GetBitmapSize();
	BitmapSize playerSize = player->GetSprite()->GetBitmapSize();
	D3DXVECTOR2 playerPosition = player->GetPosition();

	return Utils::CheckCollision(
		Utils::RECT_F{ playerPosition.x, playerPosition.y, playerPosition.x + playerSize.width, playerPosition.y + playerSize.height },
		Utils::RECT_F{ Position.x - 20, Position.y - 20, Position.x + bitmapSize.width, Position.y + bitmapSize.height }
	);
}

void Deer::RefreshCooldown()
{
	CoolDown = 0.0f;
}

bool Deer::IsDead()
{
	return Health <= 0;
}
