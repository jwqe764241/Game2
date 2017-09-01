#pragma once

#include <d3d10_1.h>

#include "Sources\Shaders\GameTexture.h"
#include "Sources\Geometries\GameBitmap.h"
#include "Sources\GameDefs.h"

class GameSprite : public GameBitmap
{
private:
	float CurrentFrame;
	float PreviousFrame;
	float MaxFrames;
	float AnimationSpeed;
	float CurrentSpeed;
	float FramePerSecond;

	float NumOfMotions;
	float CurrentMotion;

	float FrameWidth;
	float FrameHeight;

	bool IsLooping;

protected:
	void UpdateBuffers(ID3D10Device *device);

public:
	GameSprite(float framesPerSecond, float animationSpeed = 1.0f, bool isLooping = false);
	virtual ~GameSprite();

	void Initialize(ID3D10Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, float numberOfMotions);
	void Update(float dt);
	void Render(ID3D10Device* device, int screenWidth, int screenHeight, float posX, float posY);

	void SetMotion(float index);
	void SetLooping(bool condition);

	/*
		에셋 조작을 위하여 4개 함수 추가하였음
	*/
	float GetMaxFrame() const;
	float GetMotionNumber() const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;
};