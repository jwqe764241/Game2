#include <Sources/Geometries/GameBitmap.h>

GameBitmap::GameBitmap() : m_vertexBuffer(nullptr), m_indexBuffer(nullptr), m_Texture(nullptr)
{

}

GameBitmap::~GameBitmap()
{

}

bool GameBitmap::Initialize(ID3D11Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight) 
{
	m_bitmapWidth  = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	m_prevPosX = -1;
	m_prevPosY = -1;

	/* 정점 정의 */
	unsigned long* indices;

	m_vertexCount = 6;
	m_indexCount = 6;

	m_vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	float halfWidth = bitmapWidth / 2.0f;
	float halfHeight = bitmapHeight / 2.0f;

	//Triangle - 1
	m_vertices[0].position = D3DXVECTOR3(-halfWidth, halfHeight, 0.0f);
	m_vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[1].position = D3DXVECTOR3(halfWidth, -halfHeight, 0.0f);
	m_vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	m_vertices[2].position = D3DXVECTOR3(-halfWidth, -halfHeight, 0.0f);
	m_vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	//Triangle - 2
	m_vertices[3].position = D3DXVECTOR3(-halfWidth, halfHeight, 0.0f);
	m_vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	m_vertices[4].position = D3DXVECTOR3(halfWidth, halfHeight, 0.0f);
	m_vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	m_vertices[5].position = D3DXVECTOR3(halfWidth, -halfHeight, 0.0f);
	m_vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

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

bool GameBitmap::Render(ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, int posX, int posY)
{
	bool result;

	result = UpdateBuffers(deviceContext, screenWidth, screenHeight, posX, posY);
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

GameBitmap::VertexType * GameBitmap::GetVertices()
{
	return m_vertices;
}

GameBitmap::BitmapSize GameBitmap::GetBitmapSize() const
{
	return GameBitmap::BitmapSize{ m_bitmapWidth, m_bitmapHeight };
}

bool GameBitmap::InitializeBuffers(ID3D11Device *device)
{
	VertexType *vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	m_vertexCount = 6;
	m_indexCount  = 6;

	vertices = new VertexType[m_vertexCount];
	indices  = new unsigned long[m_indexCount];

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] indices;
	indices = nullptr;

	return true;
}

void GameBitmap::ReleaseBuffers()
{
	Utils::Release(&m_indexBuffer);
	Utils::Release(&m_vertexBuffer);
	ReleaseTexture();
}

bool GameBitmap::UpdateBuffers(ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, int posX, int posY)
{
	Rect_F rect;
	VertexType* vertices = GetVertices();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	if ((m_prevPosX == posX) && (m_prevPosY == posY))
	{
		return true;
	}

	m_prevPosX = posX;
	m_prevPosY = posY;

	rect.left   = static_cast<float>((screenWidth / 2) * -1) + static_cast<float>(posX);
	rect.right  = rect.left + static_cast<float>(m_bitmapWidth);
	rect.top    = static_cast<float>(screenHeight / 2) - static_cast<float>(posY);
	rect.bottom = rect.top - static_cast<float>(m_bitmapHeight);

	//Triangle - 1
	vertices[0].position.x = rect.left;
	vertices[0].position.y = rect.top;
	//vertices[0].position = D3DXVECTOR3(rect.left, rect.top, 0.0f);

	vertices[1].position.x = rect.right;
	vertices[1].position.y = rect.bottom;
	//vertices[1].position = D3DXVECTOR3(rect.right, rect.bottom, 0.0f);

	vertices[2].position.x = rect.left;
	vertices[2].position.y = rect.bottom;
	//vertices[2].position = D3DXVECTOR3(rect.left, rect.bottom, 0.0f);

	//Triangle - 2
	vertices[3].position.x = rect.left;
	vertices[3].position.y = rect.top;
	//vertices[3].position = D3DXVECTOR3(rect.left, rect.top, 0.0f);

	vertices[4].position.x = rect.right;
	vertices[4].position.y = rect.top;
	//vertices[4].position = D3DXVECTOR3(rect.right, rect.top, 0.0f);

	vertices[5].position.x = rect.right;
	vertices[5].position.y = rect.bottom;
	//vertices[5].position = D3DXVECTOR3(rect.right, rect.bottom, 0.0f);


	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	VertexType* pVertices = reinterpret_cast<VertexType*>(mappedResource.pData);

	memcpy(pVertices, reinterpret_cast<void*>(vertices), (sizeof(VertexType) * m_vertexCount));

	deviceContext->Unmap(m_vertexBuffer, 0);

	//delete[] vertices;
	//vertices = 0;

	return true;
}

void GameBitmap::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool GameBitmap::LoadTexture(ID3D11Device *device, wchar_t *filePath)
{
	bool result;

	m_Texture = new GameTexture();

	result = m_Texture->Initialize(device, filePath);
	if (!result)
	{
		return false;
	}

	return true;
}

void GameBitmap::ReleaseTexture()
{
	if (m_Texture)
	{
		//m_Texture->Release();
		//m_Texture = nullptr;
		Utils::Release(&m_Texture);
		delete m_Texture;
	}
}