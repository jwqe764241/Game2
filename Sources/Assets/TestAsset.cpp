#include <Sources/Assets/TestAsset.h>

TestAsset::TestAsset()
{
	
}

TestAsset::~TestAsset()
{

}

void TestAsset::Load(ID3D11Device* device, int bitmapWidth, int bitmapHeight)
{
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
}

void TestAsset::Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int x, int y)
{
	m_Bitmap.Render(deviceContext, screenWidth, screenHeight, x, y);
}

int TestAsset::GetIndexCount()
{
	return m_Bitmap.GetIndexCount();
}

ID3D11ShaderResourceView* TestAsset::GetTexture()
{
	return m_Bitmap.GetTexture();
}