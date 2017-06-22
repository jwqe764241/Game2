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

#include <iostream>
#include <sstream>

#include <Sources/GameDefs.h>
#include <Sources/GameWindow.h>
#include <Sources/GameTimer.h>
#include <Sources/Utils/error.h>
#include <Sources/Utils/Release.h>


class CGameApp
{
private:
	///CComPtr<ID3D11Device>			m_pD3D11Device;
	///CComPtr<ID3D11DeviceContext>	m_pD3D11DeviceContext;
	///CComPtr<IDXGISwapChain>			m_pSwapChain;
	///CComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	///CComPtr<ID3D11Texture2D>		m_pDepthStencilBuffer;
	///CComPtr<ID3D11DepthStencilView>	m_pDepthStencilView;

	//--Components
	ID3D11Device*			m_pD3D11Device;
	ID3D11DeviceContext*	m_pD3D11DeviceContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pRenderTargetView;
	ID3D11Texture2D*		m_pDepthStencilBuffer;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	CGameTimer				m_GameTimer;
	std::vector <IDXGIAdapter*> m_vAdapters;
	//--Compoenents

	//--Settings
	D3D_DRIVER_TYPE		m_DriverType;
	D3D_FEATURE_LEVEL	m_FeatureLevel;
	UINT				m_SDKVersion;
	UINT				m_MultiSampleQualityLevel;
	D3D11_VIEWPORT		m_ViewportSettings;
	//Settings--

	//--Window Val
	HWND		m_hWnd;
	HINSTANCE	m_hInstance;
	wchar_t *	m_pstrFrameTitle;
	wchar_t *	m_pstrWndClassName;
	int			m_iCmdShow;
	WINDOWSIZE  m_sizeWindow;
	//Window Val--

	//--Render Input
	ID3D11InputLayout * m_pVertex1InputLayout;
	ID3D11InputLayout * m_pVertex2InputLayout;
	
	D3DX11_PASS_DESC passDesc1;
	D3DX11_PASS_DESC passDesc2;
	//Render Input--

private:
	void CalculateFrameStatus();
public:
	void Launch();
	void Update();
	void Render();

	void onResize();
	void onShowWindow();

	LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height);
	~CGameApp();

};

#endif