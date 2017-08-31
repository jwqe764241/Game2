#include <Sources/Assets/Well.h>

void Well::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
}

void Well::Release()
{
}

void Well::Reset()
{
}

void Well::Update(float dt)
{
}

void Well::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
}

int Well::GetIndexCount()
{
	return 0;
}

ID3D10ShaderResourceView * Well::GetTexture()
{
	return nullptr;
}

D3DXVECTOR2 Well::GetPosition() const
{
	return D3DXVECTOR2();
}

void Well::SetPosition(const D3DXVECTOR2 pos)
{
}

void Well::OnAction(Player * player, float dt)
{
}

bool Well::CheckTool(Player * player)
{
	return false;
}

bool Well::CheckCollision(Player * player)
{
	return false;
}

void Well::RefreshCooldown()
{
}
