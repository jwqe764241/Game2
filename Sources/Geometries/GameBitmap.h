#pragma once

#include <d3d10_1.h>
#include <D3DX10math.h>

#include <Sources/Utils/error.h>
#include <Sources/Shaders/GameTexture.h>
#include <Sources/GameDefs.h>

class GameBitmap {
protected:
	struct _VertexType {
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};
	struct _BitmapSize
	{
		int width;
		int height;
	};

	using VertexType = _VertexType;
	using BitmapSize = _BitmapSize;

public:
	GameBitmap();
	GameBitmap(GameBitmap& other);
	~GameBitmap();

	bool Initialize(ID3D10Device *device, wchar_t* filePath, int bitmapWidth, int bitmapHeight);
	void Release();
	bool Render(ID3D10Device *device, int screenWidth, int screenHeight, int posX, int posY);

	int GetIndexCount();
	ID3D10ShaderResourceView* GetTexture();

	BitmapSize GetBitmapSize() const;
protected:
	VertexType* GetVertices();

private:
	bool InitializeBuffers(ID3D10Device* device);
	void ReleaseBuffers();
	bool UpdateBuffers(ID3D10Device *device, int screenWidth, int screenHeight, int posX, int posY);
	void RenderBuffers(ID3D10Device *device);

	bool LoadTexture(ID3D10Device *device, wchar_t *filePath);
	void ReleaseTexture();

protected:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	GameTexture *m_Texture;

	int m_bitmapWidth, m_bitmapHeight;
	int m_prevPosX   , m_prevPosY;

	VertexType* m_vertices;
};