#pragma once

#include <d3d10_1.h>
#include <D3DX10.h>

#include <Sources/Utils/error.h>
#include <Sources/GameDefs.h>

class GameTexture
{
private:
	ID3D10ShaderResourceView* Texture;

public:
	GameTexture();
	virtual ~GameTexture();

	bool Initialize(ID3D10Device* device, wchar_t* filePath);
	void Release();
	ID3D10ShaderResourceView* GetTexture();
};