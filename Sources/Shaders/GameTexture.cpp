#include "GameTexture.h"

GameTexture::GameTexture() : Texture(nullptr)
{

}

GameTexture::~GameTexture()
{

}

bool GameTexture::Initialize(ID3D10Device* device, wchar_t* filePath)
{
	GAME_ASSERT(device != nullptr && filePath != nullptr, "Arguments must not be nullptr");

	HRESULT result;
	
	result = D3DX10CreateShaderResourceViewFromFile(device, filePath, NULL, NULL, &Texture, NULL);
	if (FAILED(result)) 
	{
		return false;
	}

	return true;
}

void GameTexture::Release()
{
	Utils::Release(&Texture);
}

ID3D10ShaderResourceView* GameTexture::GetTexture()
{
	return Texture;
}