/*
	jwqe764241 ¿€º∫
*/

#ifndef __GAMEAPP_H
#define __GAMEAPP_H

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3dx11effect.h>
#include <cassert>
#include <memory>
#include <atlbase.h>
#include <xnamath.h>
#include <vector>
#include <cstring>

#include <iostream>
#include <sstream>

#include <Sources/GameDefs.h>
#include <Sources/GameWindow.h>
#include <Sources/GameTimer.h>
#include <Sources/Utils/error.h>
#include <Sources/Input/GameInput.h>

//#define TEST_RENDER_BOX

struct __ADAPTERINFO {
	LUID ID;
	std::wstring Description;
	//int Numerator;
	//int Denominator;
};

struct __APPINFO {
	ID3D11Device*			 pD3D11Device;
	ID3D11DeviceContext*	 pD3D11DeviceContext;
	IDXGISwapChain*			 pSwapChain;
	ID3D11RenderTargetView*  pRenderTargetView;
	ID3D11Texture2D*		 pBackBuffer;
	ID3D11DepthStencilState* pDepthStencilState;
	ID3D11DepthStencilState* pDepthDisableStencilState;
	ID3D11DepthStencilView*	 pDepthStencilView;
	ID3D11RasterizerState*	 pRasterizeState;
};

using ADAPTERINFO = __ADAPTERINFO;
using APPINFO = __APPINFO;

class CGameApp
{
private:
	//--Components
	APPINFO					m_AppInfo;
	CGameTimer				m_GameTimer;
	std::vector <IDXGIAdapter*> m_vAdapters;
	std::vector <IDXGIOutput *> m_vAdaptersOutputs;
	GameInput*				m_Input;
	//Compoenents--

	//--Settings
	D3D_DRIVER_TYPE		m_DriverType   = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT				m_SDKVersion = D3D11_SDK_VERSION;
	UINT				m_MultiSampleQualityLevel = 0;

	bool				m_isVsync    = false;
	bool				m_isWindowed = false;
	//Settings--

	//--Window Val
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	wchar_t *	m_pstrFrameTitle;
	wchar_t *	m_pstrWndClassName;
	int			m_iCmdShow;
	WINDOWSIZE  m_sizeWindow;
	//Window Val--

	//--Info Val
	std::vector<ADAPTERINFO> m_vAdapterInfoList;
	//Info Val--
private:
	void CalculateFrameStatus();
	void LoadAssets();

public:
	void Launch();
	void Update();
	void Render();

	void onResize();
	void onShowWindow();

	LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CGameApp() = delete;
	CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height);
	~CGameApp();

//#ifdef TEST_RENDER_BOX
//private:
//	ID3D11Buffer* m_pBoxVertexBuffer;
//	ID3D11Buffer* m_pBoxIndexBuffer;
//	void BuildBox();
//#endif
};

#endif