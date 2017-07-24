#include <Sources/Geometries/GameBitmap.h>

GameBitmap::GameBitmap() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_Texture(nullptr)
{

}

GameBitmap::GameBitmap(const GameBitmap& other) 
{

}

GameBitmap::~GameBitmap()
{

}

bool GameBitmap::Initialize(ID3D11Device *device, int screenWidth, int screenHeight, wchar_t *filePath, int bitmapWidth, int bitmapHeight) 
{
	m_screenWidth  = screenWidth;
	m_screenHeight = screenHeight;

	m_bitmapWidth  = bitmapWidth;
	m_bitmapHeight = bitmapHeight;


	m_prevPosX = -1;
	m_prevPosY = -1;

	bool result;
	result = InitializeBuffers(device);
	if (!result) 
	{
		return false;
	}

	result = LoadTexture(device, filePath);
	if (!result) 
	{
		return false;
	}

	return true;
}

void GameBitmap::Release() 
{
	ReleaseTexture();
	ReleaseBuffers();
}

bool GameBitmap::Render(ID3D11DeviceContext *deviceContext, int posX, int posY)
{
	bool result;

	result = UpdateBuffers(deviceContext, posX, posY);
	if (!result) 
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

int GameBitmap::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* GameBitmap::GetTexture()
{
	return m_Texture->GetTexture();
}


bool GameBitmap::InitializeBuffers(ID3D11Device *device)
{
	VertexType *vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	m_vertexCount = 6;
	m_indexCount  = 6;

	vertices = new VertexType[m_vertexCount];
	indices  = new unsigned long[m_indexCount];

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}


	return true;
}

void GameBitmap::ReleaseBuffers()
{

}

bool GameBitmap::UpdateBuffers(ID3D11DeviceContext *deviceContext, int posX, int posY)
{
	return true;
}

void GameBitmap::RenderBuffers(ID3D11DeviceContext *deviceContext)
{

}

bool GameBitmap::LoadTexture(ID3D11Device *device, wchar_t *filePath)
{
	return true;
}

void GameBitmap::ReleaseTexture()
{

}