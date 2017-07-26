#ifndef __GAMEDEFS_H
#define __GAMEDEFS_H

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <xnamath.h>

namespace GameColors {
	const XMVECTORF32 _RED     = { 1.0f, 0.0f, 0.0f, 1.0f };
	const XMVECTORF32 _GREEN   = { 0.0f, 1.0f, 0.0f, 1.0f };
	const XMVECTORF32 _BLUE    = { 0.0f, 0.0f, 1.0f, 1.0f };
	const XMVECTORF32 _BLACK   = { 0.0f, 0.0f, 0.0f, 1.0f };
	const XMVECTORF32 _WHITE   = { 1.0f, 1.0f, 1.0f, 1.0f };
	const XMVECTORF32 _YELLOW  = { 1.0f, 1.0f, 0.0f, 1.0f };
	const XMVECTORF32 _CYAN    = { 0.0f, 1.0f, 1.0f, 1.0f };
	const XMVECTORF32 _MAGENTA = { 1.0f, 0.0f, 1.0f, 1.0f };

	static auto RED     = reinterpret_cast<const float *>(&_RED);
	static auto GREEN   = reinterpret_cast<const float *>(&_GREEN);
	static auto BLUE    = reinterpret_cast<const float *>(&_BLUE);
	static auto BLACK   = reinterpret_cast<const float *>(&_BLACK);
	static auto WHITE   = reinterpret_cast<const float *>(&_WHITE);
	static auto YELLOW  = reinterpret_cast<const float *>(&_YELLOW);
	static auto CYAN    = reinterpret_cast<const float *>(&_CYAN);
	static auto MAGENTA = reinterpret_cast<const float *>(&_MAGENTA);
}

//이제 사용안함
//namespace Vertex {
//	struct Vertex1 {
//		XMFLOAT3 Position;
//		XMFLOAT4 Color;
//	};
//
//	struct Vertex2 {
//		XMFLOAT3 Position;
//		XMFLOAT3 Normal;
//		XMFLOAT2 Texture0;
//		XMFLOAT2 Texture1;
//	};
//
//	const D3D11_INPUT_ELEMENT_DESC Desc1[] = {
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	const D3D11_INPUT_ELEMENT_DESC Desc2[] = {
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//}

namespace Utils {
	template <typename T>
	void Release(T ** pTy) {
		if ((*pTy)) {
			(*pTy)->Release();
			(*pTy) = nullptr;
		}
	}
}

#endif