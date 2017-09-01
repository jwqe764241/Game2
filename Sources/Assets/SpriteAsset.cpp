#include "SpriteAsset.h"

SpriteAsset::SpriteAsset() : m_Object(15.0f, 1.0f, true)
{
}

SpriteAsset::~SpriteAsset()
{
}

void SpriteAsset::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
	m_PosX = 20;
	m_PosY = 20;
	m_Object.Initialize(device, ResourcePath, bitmapWidth, bitmapHeight, 4);
}

void SpriteAsset::Release()
{
	m_Object.Release();
}

void SpriteAsset::Reset()
{
}

void SpriteAsset::Update(float dt)
{
	m_Object.Update(dt);
}

void SpriteAsset::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	m_Object.Render(device, screenWidth, screenHeight, m_PosX, m_PosY);
}

int SpriteAsset::GetIndexCount()
{
	return m_Object.GetIndexCount();
}

ID3D10ShaderResourceView * SpriteAsset::GetTexture()
{
	return m_Object.GetTexture();
}

D3DXVECTOR2 SpriteAsset::GetPosition() const
{
	return D3DXVECTOR2(m_PosX, m_PosY);
}

void SpriteAsset::SetPosition(const D3DXVECTOR2 pos)
{
	m_PosX = pos.x;
	m_PosY = pos.y;
}
