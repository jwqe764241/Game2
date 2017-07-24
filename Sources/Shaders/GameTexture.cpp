#include "GameTexture.h"

GameTexture::GameTexture() : m_Texture(nullptr)
{

}

GameTexture::GameTexture(const GameTexture& other) : GameTexture()
{

}

GameTexture::~GameTexture()
{

}

bool GameTexture::Initialize(ID3D11Device* device, wchar_t* filePath)
{
	GAME_ASSERT(device != nullptr && filePath != nullptr, "Arguments must not be nullptr");

	HRESULT result;
	
	result = D3DX11CreateShaderResourceViewFromFile(device, filePath, NULL, NULL, &m_Texture, NULL);
	if (FAILED(result)) 
	{
		return false;
	}

	return true;
}

void GameTexture::Release()
{
	Utils::Release(&m_Texture);
}

ID3D11ShaderResourceView* GameTexture::GetTexture()
{
	return m_Texture;
}