#include <Sources/Assets/TestAsset.h>

TestAsset::TestAsset()
{
	
}

TestAsset::TestAsset(TestAsset& other)
{
}

TestAsset::~TestAsset()
{

}

void TestAsset::Load(ID3D10Device* device, int bitmapWidth, int bitmapHeight)
{
	m_PosX = 0;
	m_PosY = 0;
	m_Bitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight);
}

void TestAsset::Release()
{
	m_Bitmap.Release();
}

void TestAsset::Reset()
{

}

void TestAsset::Update(float dt)
{
	//m_PosX += 1;
	//m_PosY += 1;
}

void TestAsset::Render(ID3D10Device* device, int screenWidth, int screenHeight)
{
	m_Bitmap.Render(device, screenWidth, screenHeight, m_PosX, m_PosY);
}

int TestAsset::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D10ShaderResourceView* TestAsset::GetTexture()
{
	return m_Bitmap.GetTexture();
}

D3DXVECTOR2 TestAsset::GetPosition() const
{
	return D3DXVECTOR2{ m_PosX, m_PosY };
}

void TestAsset::SetPosition(const D3DXVECTOR2 pos)
{
	m_PosX = pos.x;
	m_PosY = pos.y;
}

GameSprite* TestAsset::GetSprite()
{
	return nullptr;
}