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
#include <D3DX10math.h>

#include <iostream>
#include <sstream>

#include <Sources/GameDefs.h>
#include <Sources/GameWindow.h>
#include <Sources/GameTimer.h>
#include <Sources/Utils/error.h>
#include <Sources/Shaders/TextureShader.h>
#include <Sources/Shaders/GameTexture.h>
#include <Sources/Geometries/GameBitmap.h>
#include <Sources/Camera/GameCamera.h>

//#define TEST_RENDER_BOX

class CGameApp
{
private:
	struct _AdapterInfo {
		LUID ID;
		std::wstring Description;
		//int Numerator;
		//int Denominator;
	};

	struct _AppInfo {
		ID3D11Device			*pD3D11Device;
		ID3D11DeviceContext		*pD3D11DeviceContext;
		IDXGISwapChain			*pSwapChain;
		ID3D11RenderTargetView	*pRenderTargetView;
		ID3D11Texture2D			*pBackBuffer;
		ID3D11DepthStencilState *pDepthStencilState;
		ID3D11DepthStencilState *pDepthDisableStencilState;
		ID3D11DepthStencilView	*pDepthStencilView;
		ID3D11RasterizerState	*pRasterizeState;
	};

	struct _WindowSize
	{
		int width;
		int height;
	};

	struct _Matrix
	{
		D3DXMATRIX projectionMatrix;
		D3DXMATRIX worldMatrix;
		D3DXMATRIX orthMatrix;
	};

	using AdapterInfo = _AdapterInfo;
	using AppInfo     = _AppInfo;
	using WindowSize  = _WindowSize;
	using Matrix      = _Matrix;

private:
	//--Components
	AppInfo		m_AppInfo;
	CGameTimer	m_GameTimer;
	Matrix		m_Matrix;
	std::vector <IDXGIAdapter*> m_vAdapters;
	std::vector <IDXGIOutput *> m_vAdaptersOutputs;
	//Compoenents--

	//--Settings
	D3D_DRIVER_TYPE		m_DriverType   = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL	m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT				m_SDKVersion = D3D11_SDK_VERSION;
	UINT				m_MultiSampleQualityLevel = 0;

	bool m_isVsync    = false;
	bool m_isWindowed = false;
	float m_screenDepth;
	float m_screenNear;
	//Settings--

	//--Window Val
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	wchar_t		*m_pstrFrameTitle;
	wchar_t		*m_pstrWndClassName;
	int			m_iCmdShow;
	WindowSize  m_sizeWindow;
	//Window Val--

	//--Info Val
	std::vector<AdapterInfo> m_vAdapterInfoList;
	//Info Val--

	CGameCamera* m_Camera;
	TextureShader* m_TextureShader;
	GameBitmap*	m_Bitmap;
private:
	void CalculateFrameStatus();
	void LoadAssets();

public:
	void Launch();
	void Update();
	void Render();

	void onResize();
	void onShowWindow();

	void TurnOnZBuffer();
	void TurnOffZBuffer();

	D3DXMATRIX& GetProjectionMatrix();
	D3DXMATRIX& GetWorldMatrix();
	D3DXMATRIX& GetorthogonalMatrix();

	LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CGameApp() = delete;
	CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height, float screenDepth, float screenNear);
	~CGameApp();

};

#endif