#pragma once

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <cassert>
#include <memory>
#include <atlbase.h>

#include <Sources/GameWindow.h>
#include <Sources/Utils/error.h>

class CGameApp
{
private:
	CComPtr<ID3D11Device>			m_pD3D11Device;
	CComPtr<ID3D11DeviceContext>	m_pD3D11DeviceContext;
	CGameWindow*					m_pGameWindow;
	IDXGISwapChain*					m_pSwapChain;
	ID3D11RenderTargetView*			m_pRenderTargetView;

	//--Settings
	D3D_DRIVER_TYPE		m_DriverType;
	D3D_FEATURE_LEVEL	m_FeatureLevel;
	UINT				m_SDKVersion;
	UINT				m_MultiSampleQualityLevel;
	//Settings--

private:


public:
	void Launch();

public:
	CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height);
	~CGameApp();

};

