#include <Sources/Assets/Workshop.h>

void Workshop::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void Workshop::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
}

void Workshop::Release()
{
}

void Workshop::Reset()
{
}

void Workshop::Update(float dt)
{
}

void Workshop::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
}

int Workshop::GetIndexCount()
{
	return 0;
}

ID3D10ShaderResourceView * Workshop::GetTexture()
{
	return nullptr;
}

D3DXVECTOR2 Workshop::GetPosition() const
{
	return D3DXVECTOR2();
}

void Workshop::SetPosition(const D3DXVECTOR2 pos)
{
}

void Workshop::OnAction(Player * player, float dt)
{
}

bool Workshop::CheckTool(Player * player)
{
	return false;
}

bool Workshop::CheckCollision(Player * player)
{
	return false;
}

void Workshop::RefreshCooldown()
{
}
