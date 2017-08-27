#pragma once

#include <Sources/Geometries/GameBitmap.h>

class ITools {
public:
	virtual void Render() = 0;
	virtual GameBitmap& GetTexture() = 0;
};