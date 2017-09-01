#pragma once

#include <Sources/Geometries/GameBitmap.h>

class StateBar : public GameBitmap 
{
private:
	int MaximumValue;
	int CurrentValue;
private:
	bool UpdateBuffers(ID3D10Device *device, int screenWidth, int screenHeight, int posX, int posY);
	void RenderBuffers(ID3D10Device *device);
public:
	StateBar();
	virtual ~StateBar();

	void Initialize(ID3D10Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, int maxValue, int curValue);
	bool Render(ID3D10Device *device, int screenWidth, int screenHeight, int posX, int posY);
	void Release();

	void SetValue(int value);
};