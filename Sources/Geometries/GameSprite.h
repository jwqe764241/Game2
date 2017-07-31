#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

#include "Sources\Shaders\GameTexture.h"
#include "Sources\Geometries\GameBitmap.h"
#include "Sources\GameDefs.h"

class GameSprite : public GameBitmap
{
public:
	GameSprite(float framesPerSecond, float animationSpeed = 1.0f, bool isLooping = false);
	virtual ~GameSprite();

	void Initialize(ID3D11Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, float numberOfMotions);
	void Update(float dt);
	void Render(ID3D11DeviceContext* context, int posX, int posY);

protected:
	void UpdateBuffers(ID3D11DeviceContext *deviceContext);

private:
	float m_currentFrame;
	float m_previousFrame;
	float m_maxFrames;
	float m_animationSpeed;
	float m_currentSpeed;
	float m_framesPerSecond;

	float m_numOfMotions;
	float m_currentMotion;

	bool m_isLooping;
};