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

void GameSprite::Initialize(ID3D10Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, float numberOfMotions)
{
	m_currentFrame = 0;
	m_previousFrame = -1.0f;
	m_maxFrames = (float)bitmapWidth / (float)(bitmapHeight / numberOfMotions);
	m_numOfMotions = numberOfMotions;

	m_frameWidth = bitmapWidth / m_maxFrames;
	m_frameHeight = bitmapHeight / m_numOfMotions;

	GameBitmap::Initialize(device, filePath, bitmapWidth / m_maxFrames, bitmapHeight / numberOfMotions);
}

void GameSprite::Update(float dt)
{
	if (m_maxFrames == 1.0f) return;

	/*
		전방에 체크를 하지 않으면
		m_isLooping 값을 초기에 false 값으로 설정을 해 놓아도
		마지막 프레임까지 업데이트 되고 설정이 되기 때문에
		해당 위치에 추가하게됨
	*/
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
				//초기 체크 위치
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
void GameSprite::Render(ID3D10Device * device, int screenWidth, int screenHeight, float posX, float posY)
{
	// 위치 정보를 업데이트하고, 렌더한다.
	GameBitmap::Render(device, screenWidth, screenHeight, posX, posY);
	// 스프라이트의 애니메이션(프레임)을 업데이트한다.
	UpdateBuffers(device);

	/*
		이거 뭐냐 왜한거임 ㅡㅡ
	*/
	// 렌더한다.
	//unsigned int stride = sizeof(VertexType);
	//unsigned int offset = 0;

	//device->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//device->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

float GameSprite::GetMaxFrame() const
{
	return m_maxFrames;
}

float GameSprite::GetMotionNumber() const
{
	return m_numOfMotions;
}

float GameSprite::GetFrameWidth() const
//그려질 프레임의 너비
{
	return m_frameWidth;
}

float GameSprite::GetFrameHeight() const
//그려질 프레임의 높이
{
	return m_frameHeight;
}

void GameSprite::UpdateBuffers(ID3D10Device * deviceContext)
{
	// 이전 프레임과 동일한 프레임인 경우. 함수를 나온다.
	HRESULT result;

	if (m_currentFrame == m_previousFrame) return;
	
	VertexType* vertices = GameBitmap::GetVertices();

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

	void* pVertices;
	result = m_vertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, &pVertices);
	if (FAILED(result)) return;

	int verticesSize = sizeof(VertexType) * m_vertexCount;
	memcpy_s(pVertices, verticesSize, (void*)vertices, verticesSize);

	m_vertexBuffer->Unmap();

	m_previousFrame = m_currentFrame;
}
