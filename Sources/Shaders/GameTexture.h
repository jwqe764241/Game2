#pragma once

#include <D3DX11.h>

#include <Sources/Utils/error.h>
#include <Sources/GameDefs.h>

class GameTexture{
public:
	GameTexture();
	GameTexture(const GameTexture& other);
	~GameTexture();

	bool Initialize(ID3D11Device* device, wchar_t* filePath);
	void Release();
	ID3D11ShaderResourceView* GetTexture();

	ID3D11ShaderResourceView* m_Texture;

};