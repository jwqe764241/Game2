#pragma once

#include <d3d10_1.h>
#include <D3DX10.h>
#include <cstring>

class GameTextWriter
{
private:
	ID3DX10Font* Font;
	ID3DX10Sprite* Sprite;

public:
	GameTextWriter();
	virtual ~GameTextWriter();

	bool Initialize(ID3D10Device* pDevice, int height, int width);
	void DrawString(wchar_t* string, RECT rect, UINT format, D3DXCOLOR color);
};