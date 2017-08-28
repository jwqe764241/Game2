#pragma once

#include <d3d10_1.h>
#include <D3DX10.h>

#include <Sources/Utils/error.h>
#include <Sources/GameDefs.h>

class GameTexture{
public:
	GameTexture();
	GameTexture(const GameTexture& other);
	~GameTexture();

	bool Initialize(ID3D10Device* device, wchar_t* filePath);
	void Release();
	ID3D10ShaderResourceView* GetTexture();

	ID3D10ShaderResourceView* m_Texture;

};