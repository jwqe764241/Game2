#pragma once

#include <Windows.h>
#include <xnamath.h>

struct Vertex1 {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

struct Vertex2 {
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 Texture0;
	XMFLOAT2 Texture1;
};
