#pragma once

#include <Sources/Geometries/GameBitmap.h>

class StateBar : public GameBitmap {
private:
	int maxValue;
	int curValue;
private:
	bool UpdateBuffers(ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, int posX, int posY);
	void RenderBuffers(ID3D11DeviceContext *deviceContext);
public:
	void Initialize(ID3D11Device *device, wchar_t *filePath, int bitmapWidth, int bitmapHeight, int maxValue, int curValue);
	bool Render(ID3D11DeviceContext *deviceContext, int screenWidth, int screenHeight, int posX, int posY);
	void Release();

	void SetValue(int value);
};