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

void TestAsset::Load(ID3D11Device* device, int bitmapWidth, int bitmapHeight)
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

void TestAsset::Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	m_Bitmap.Render(deviceContext, screenWidth, screenHeight, m_PosX, m_PosY);
}

int TestAsset::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D11ShaderResourceView* TestAsset::GetTexture()
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