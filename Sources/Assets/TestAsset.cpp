#include <Sources/Assets/TestAsset.h>

TestAsset::TestAsset()
{
	
}

TestAsset::~TestAsset()
{

}

void TestAsset::Load(ID3D11Device* device, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight)
{
	m_Bitmap.Initialize(device, screenWidth, screenHeight, filePath, bitmapWidth, bitmapHeight);
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
}

void TestAsset::Render(ID3D11DeviceContext* deviceContext, int x, int y)
{
	m_Bitmap.Render(deviceContext, x, y);
}

int TestAsset::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D11ShaderResourceView* TestAsset::GetTexture()
{
	return m_Bitmap.GetTexture();
}