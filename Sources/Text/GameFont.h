#pragma once

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>

class GameFont
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	GameFont();
	GameFont(const GameFont&);
	~GameFont();

	bool Initialize(ID3D11Device*, char*, wchar_t*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, wchar_t*);
	void ReleaseTexture();

private:
	FontType* m_font;
	
};