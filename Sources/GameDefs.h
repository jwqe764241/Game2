#ifndef __GAMEDEFS_H
#define __GAMEDEFS_H

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>

namespace GameColors {
	constexpr FLOAT RED[4]     = {1.0f, 0.0f, 0.0f, 1.0f};
	constexpr FLOAT GREEN[4]   = {0.0f, 1.0f, 0.0f, 1.0f};
	constexpr FLOAT BLUE[4]    = {0.0f, 0.0f, 1.0f, 1.0f};
	constexpr FLOAT BLACK[4]   = {0.0f, 0.0f, 0.0f, 1.0f};
	constexpr FLOAT WHITE[4]   = {1.0f, 1.0f, 1.0f, 1.0f};
	constexpr FLOAT YELLOW[4]  = {1.0f, 1.0f, 0.0f, 1.0f};
	constexpr FLOAT CYAN[4]    = {0.0f, 1.0f, 1.0f, 1.0f};
	constexpr FLOAT MAGENTA[4] = {1.0f, 0.0f, 1.0f, 1.0f};
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