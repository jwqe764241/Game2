#include "SpriteAsset.h"

SpriteAsset::SpriteAsset() : m_object(15.0f, 1.0f, true)
{
}

SpriteAsset::~SpriteAsset()
{
}

void SpriteAsset::Load(ID3D11Device * device, int bitmapWidth, int bitmapHeight)
{
	m_object.Initialize(device, m_filePath, bitmapWidth, bitmapHeight, 4);
}

void SpriteAsset::Release()
{
	m_object.Release();
}

void SpriteAsset::Reset()
{
}

void SpriteAsset::Update(float dt)
{
	m_object.Update(dt);
}

void SpriteAsset::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight, int x, int y)
{
	m_object.Render(deviceContext, screenWidth, screenHeight, x, y);
}

int SpriteAsset::GetIndexCount()
{
	return m_object.GetIndexCount();
}

ID3D11ShaderResourceView * SpriteAsset::GetTexture()
{
	return m_object.GetTexture();
}
