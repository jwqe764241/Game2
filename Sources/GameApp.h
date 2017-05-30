#pragma once


#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <cassert>
#include <memory>

#include <Sources/Utils/error.h>

class CGameApp 
{
private:
	ID3D11Device		* m_pD3D11Device;
	ID3D11DeviceContext * m_pD3D11DeviceContext;

	D3D_DRIVER_TYPE		m_DriveLevel = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL	m_FeatureLevel;
	UINT				m_SDKVersion = D3D11_SDK_VERSION;

	//--Window
	HWND m_hWnd;
	//Window--
private:


public:


public:
	CGameApp();
	~CGameApp();

};

