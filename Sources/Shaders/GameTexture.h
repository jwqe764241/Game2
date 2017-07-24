#ifndef __GAMETEXTURE_H
#define __GAMETEXTURE_H

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <memory>
#include <atlbase.h>
#include <xnamath.h>
#include <D3DX10math.h>

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

#endif