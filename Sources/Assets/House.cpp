#include <Sources/Assets/House.h>

void House::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
}

void House::Load(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight, int x, int y)
{
	m_Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);
	
	m_Position.x = x;
	m_Position.y = y;
}

void House::Release()
{
}

void House::Reset()
{
}

void House::Update(float dt)
{
}

void House::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
}

int House::GetIndexCount()
{
	return 0;
}

ID3D10ShaderResourceView * House::GetTexture()
{
	return nullptr;
}

D3DXVECTOR2 House::GetPosition() const
{
	return D3DXVECTOR2();
}

void House::SetPosition(const D3DXVECTOR2 pos)
{
}

void House::OnAction(Player * player, float dt)
{
}

bool House::CheckTool(Player * player)
{
	return false;
}

bool House::CheckCollision(Player * player)
{
	return false;
}

void House::RefreshCooldown()
{
}
