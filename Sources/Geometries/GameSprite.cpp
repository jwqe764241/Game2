#include "GameSprite.h"

GameSprite::GameSprite(float framesPerSecond, float animationSpeed, bool isLooping) : GameBitmap()
{
	FramePerSecond = 1 / framesPerSecond;
	AnimationSpeed = animationSpeed;
	IsLooping = isLooping;
	CurrentSpeed = 0;
	CurrentMotion = 0;
}

GameSprite::~GameSprite()
{
}

void GameSprite::Initialize(ID3D10Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, float numberOfMotions)
{
	CurrentFrame = 0;
	PreviousFrame = -1.0f;
	MaxFrames = (float)bitmapWidth / (float)(bitmapHeight / numberOfMotions);
	NumOfMotions = numberOfMotions;

	FrameWidth = bitmapWidth / MaxFrames;
	FrameHeight = bitmapHeight / NumOfMotions;

	GameBitmap::Initialize(device, filePath, bitmapWidth / MaxFrames, bitmapHeight / numberOfMotions);
}

void GameSprite::Update(float dt)
{
	if (MaxFrames == 1.0f) return;

	/*
		전방에 체크를 하지 않으면
		IsLooping 값을 초기에 false 값으로 설정을 해 놓아도
		마지막 프레임까지 업데이트 되고 설정이 되기 때문에
		해당 위치에 추가하게됨
	*/
	if (!IsLooping)
	{
		CurrentFrame = MaxFrames;
		return;
	}

	if (CurrentFrame <= MaxFrames) {
		CurrentSpeed += AnimationSpeed * dt;

		if (CurrentSpeed > FramePerSecond) {
			CurrentFrame++;
			CurrentSpeed = 0;

			if (CurrentFrame >= MaxFrames) {
				if (IsLooping) {
					CurrentFrame = 0;
				}
				//초기 체크 위치
				//else {
				//	CurrentFrame = MaxFrames;
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
	if (CurrentMotion != index)
	{
		CurrentMotion = index;
	}
}

void GameSprite::SetLooping(bool condition)
{
	if (IsLooping != condition)
	{
		IsLooping = condition;
	}
}

float GameSprite::GetMaxFrame() const
{
	return MaxFrames;
}

float GameSprite::GetMotionNumber() const
{
	return NumOfMotions;
}

float GameSprite::GetFrameWidth() const
//그려질 프레임의 너비
{
	return FrameWidth;
}

float GameSprite::GetFrameHeight() const
//그려질 프레임의 높이
{
	return FrameHeight;
}

void GameSprite::UpdateBuffers(ID3D10Device * deviceContext)
{
	// 이전 프레임과 동일한 프레임인 경우. 함수를 나온다.
	HRESULT result;

	if (CurrentFrame == PreviousFrame) return;
	
	VertexType* vertices = GameBitmap::GetVertices();

	vertices[0].texture.x = CurrentFrame / MaxFrames;
	vertices[0].texture.y = CurrentMotion / NumOfMotions;

	vertices[1].texture.x = (CurrentFrame + 1.0) / MaxFrames;
	vertices[1].texture.y = (CurrentMotion + 1.0) / NumOfMotions;

	vertices[2].texture.x = CurrentFrame / MaxFrames;
	vertices[2].texture.y = (CurrentMotion + 1.0) / NumOfMotions;

	vertices[3].texture.x = CurrentFrame / MaxFrames;
	vertices[3].texture.y = CurrentMotion / NumOfMotions;

	vertices[4].texture.x = (CurrentFrame + 1.0) / MaxFrames;
	vertices[4].texture.y = CurrentMotion / NumOfMotions;

	vertices[5].texture.x = (CurrentFrame + 1.0) / MaxFrames;
	vertices[5].texture.y = (CurrentMotion + 1.0) / NumOfMotions;

	void* pVertices;
	result = m_vertexBuffer->Map(D3D10_MAP_WRITE_DISCARD, NULL, &pVertices);
	if (FAILED(result)) return;

	int verticesSize = sizeof(VertexType) * m_vertexCount;
	memcpy_s(pVertices, verticesSize, (void*)vertices, verticesSize);

	m_vertexBuffer->Unmap();

	PreviousFrame = CurrentFrame;
}
