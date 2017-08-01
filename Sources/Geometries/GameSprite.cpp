#include "GameSprite.h"

GameSprite::GameSprite(float framesPerSecond, float animationSpeed, bool isLooping) : GameBitmap()
{
	m_framesPerSecond = 1 / framesPerSecond;
	m_animationSpeed = animationSpeed;
	m_isLooping = isLooping;
	m_currentSpeed = 0;
	m_currentMotion = 0;
}

GameSprite::~GameSprite()
{
}

void GameSprite::Initialize(ID3D11Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, float numberOfMotions)
{
	m_currentFrame = 0;
	m_previousFrame = -1.0f;
	m_maxFrames = (float)bitmapWidth / (float)(bitmapHeight / numberOfMotions);
	m_numOfMotions = numberOfMotions;

	GameBitmap::Initialize(device, filePath, bitmapWidth / m_maxFrames, bitmapHeight / numberOfMotions);
}

void GameSprite::Update(float dt)
{
	if (m_maxFrames == 1.0f) return;

	if (!m_isLooping)
	{
		m_currentFrame = m_maxFrames;
		return;
	}

	if (m_currentFrame <= m_maxFrames) {
		m_currentSpeed += m_animationSpeed * dt;

		if (m_currentSpeed > m_framesPerSecond) {
			m_currentFrame++;
			m_currentSpeed = 0;

			if (m_currentFrame >= m_maxFrames) {
				if (m_isLooping) {
					m_currentFrame = 0;
				}
				//else {
				//	m_currentFrame = m_maxFrames;
				//}
			}
		}
	}
}

/*
	스프라이트의 위치 및 애니메이션(프레임)의 정점을 업데이트하고, 렌더한다.
	단, 현재 이 작업의 진행과정이 다소 비효율적이다.
		1. 버퍼 수정 작업을 2번 수행한다. 한 오브젝트마다~!
		2. 그에 따라 렌더 작업도 2번 수행한다.
	추후에 이 것을 수정해야될 듯 싶다.
*/
void GameSprite::Render(ID3D11DeviceContext* context, int screenWidth, int screenHeight, int posX, int posY)
{
	// 위치 정보를 업데이트하고, 렌더한다.
	GameBitmap::Render(context, screenWidth, screenHeight, posX, posY);

	// 스프라이트의 애니메이션(프레임)을 업데이트한다.
	UpdateBuffers(context);

	// 렌더한다.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GameSprite::SetMotion(float index)
{
	if (m_currentMotion != index)
	{
		m_currentMotion = index;
	}
}

void GameSprite::SetLooping(bool condition)
{
	if (m_isLooping != condition)
	{
		m_isLooping = condition;
	}
}

void GameSprite::UpdateBuffers(ID3D11DeviceContext * deviceContext)
{
	// 이전 프레임과 동일한 프레임인 경우. 함수를 나온다.
	if (m_currentFrame == m_previousFrame) return;
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GameBitmap::VertexType* vertices = GameBitmap::GetVertices();

	vertices[0].texture.x = m_currentFrame / m_maxFrames;
	vertices[0].texture.y = m_currentMotion / m_numOfMotions;

	vertices[1].texture.x = (m_currentFrame + 1.0) / m_maxFrames;
	vertices[1].texture.y = (m_currentMotion + 1.0) / m_numOfMotions;

	vertices[2].texture.x = m_currentFrame / m_maxFrames;
	vertices[2].texture.y = (m_currentMotion + 1.0) / m_numOfMotions;

	vertices[3].texture.x = m_currentFrame / m_maxFrames;
	vertices[3].texture.y = m_currentMotion / m_numOfMotions;

	vertices[4].texture.x = (m_currentFrame + 1.0) / m_maxFrames;
	vertices[4].texture.y = m_currentMotion / m_numOfMotions;

	vertices[5].texture.x = (m_currentFrame + 1.0) / m_maxFrames;
	vertices[5].texture.y = (m_currentMotion + 1.0) / m_numOfMotions;

	HRESULT result;
	result = deviceContext->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	if (FAILED(result)) return;

	GameBitmap::VertexType* pVertices = reinterpret_cast<GameBitmap::VertexType*>(mappedResource.pData);

	int verticesSize = sizeof(GameBitmap::VertexType) * m_vertexCount;
	memcpy_s(pVertices, verticesSize, (void*)vertices, verticesSize);

	deviceContext->Unmap(m_vertexBuffer, NULL);

	m_previousFrame = m_currentFrame;
}
