#include <Sources/Geometries/StateBar.h>

void StateBar::Initialize(ID3D11Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, int maxValue, int curValue)
{
	this->maxValue = maxValue;
	this->curValue = curValue;

	GameBitmap::Initialize(device, filePath, bitmapWidth, bitmapHeight);
}

bool StateBar::Render(ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, int posX, int posY)
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

void StateBar::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool StateBar::UpdateBuffers(ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, int posX, int posY)
{
	Rect_F rect;
	VertexType* vertices = GetVertices();
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	//if ((m_prevPosX == posX) && (m_prevPosY == posY))
	//{
	//	return true;
	//}

	m_prevPosX = posX;
	m_prevPosY = posY;

	rect.left = static_cast<float>((screenWidth / 2) * -1) + static_cast<float>(posX);
	rect.right = rect.left + static_cast<float>(2 * curValue);
	rect.top = static_cast<float>(screenHeight / 2) - static_cast<float>(posY);
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

void StateBar::Release()
{
	GameBitmap::Release();
}

void StateBar::SetValue(int value)
{
	if (value >= maxValue)
	{
		curValue = maxValue;
	}
	else if (value <= 0)
	{
		curValue = 0;
	}
	else
	{
		curValue = value;
	}
}