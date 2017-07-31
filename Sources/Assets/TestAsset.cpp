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
	m_posX = 20;
	m_posY = 20;
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
	//m_posX += 1;
	//m_posY += 1;
}

void TestAsset::Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	m_Bitmap.Render(deviceContext, screenWidth, screenHeight, m_posX, m_posY);
}

int TestAsset::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D11ShaderResourceView* TestAsset::GetTexture()
{
	return m_Bitmap.GetTexture();
}