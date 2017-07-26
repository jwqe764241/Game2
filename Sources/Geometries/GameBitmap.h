#ifndef __TESTGEOMETRY_H
#define __TESTGEOMETRY_H

#include <D3DX11.h>
#include <D3DX10math.h>

#include <Sources/Utils/error.h>
#include <Sources/Shaders/GameTexture.h>

class GameBitmap {
private:
	struct _VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct _RECT_F {
		float left;
		float right;
		float top;
		float bottom;
	};

	using VertexType = _VertexType;
	using Rect_F     = _RECT_F;

public:
	GameBitmap();
	GameBitmap(const GameBitmap& other);
	~GameBitmap();

	bool Initialize(ID3D11Device *device, int screenWidth, int screenHeight, wchar_t* filePath, int bitmapWidth, int bitmapHeight);
	void Release();
	bool Render(ID3D11DeviceContext *deviceContext, int posX, int posY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitializeBuffers(ID3D11Device* device);
	void ReleaseBuffers();
	bool UpdateBuffers(ID3D11DeviceContext *deviceContext, int posX, int posY);
	void RenderBuffers(ID3D11DeviceContext *deviceContext);

	bool LoadTexture(ID3D11Device *device, wchar_t *filePath);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	GameTexture *m_Texture;

	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_prevPosX   , m_prevPosY;
};

#endif