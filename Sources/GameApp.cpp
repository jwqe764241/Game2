#include <Sources/GameApp.h>
#include <Sources/Utils/Release.h>

#include <iostream>
#include <sstream>


HRESULT hr;

CGameApp::CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height)
	: m_pD3D11Device(nullptr), m_pD3D11DeviceContext(nullptr), m_pGameWindow(nullptr), m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr), m_pDepthStencilBuffer(nullptr), m_pDepthStencilView(nullptr),
	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),m_SDKVersion(D3D11_SDK_VERSION), m_MultiSampleQualityLevel(NULL)
{

	m_pGameWindow = new CGameWindow(hInstance, frameTitle, wndClassName, nCmdShow, width, height);

	UINT createFlags = 0;
#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	CHECK_HR(D3D11CreateDevice(
		0,
		m_DriverType,
		0,
		createFlags,
		NULL, NULL,
		m_SDKVersion,
		&m_pD3D11Device,
		&m_FeatureLevel,
		&m_pD3D11DeviceContext
	));

	if (FAILED(hr)) {
		GAME_ASSERT(0 != 0, "Failed - D3D11CreateDevice");

		if (MessageBox(NULL, L"Failed - D3D11CreateDevice", L"ERROR!!", MB_OK) == IDOK) {
			if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
				PostQuitMessage(0);
			}
		}
	}
	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0) {
		GAME_ASSERT(0 != 0, "Failed - Not Support Directx 11");

		if (MessageBox(NULL, L"Failed - Not Support Directx 11", L"ERROR!!", MB_OK) == IDOK) {
			if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
				PostQuitMessage(0);
			}
		}
	}


	if (m_pD3D11Device != nullptr) {
		CHECK_HR(m_pD3D11Device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MultiSampleQualityLevel
		));
		GAME_ASSERT(m_MultiSampleQualityLevel > 0, "Unexpected ERROR");
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width					 = m_pGameWindow->GetWidth();
	swapChainDesc.BufferDesc.Height					 = m_pGameWindow->GetHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator	 = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format					 = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering		 = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling				 = DXGI_MODE_SCALING_UNSPECIFIED;
	
	if (m_MultiSampleQualityLevel) {
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
	}
	else {
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_pGameWindow->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	
	IDXGIDevice * dxgiDevice = nullptr;
	CHECK_HR(m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)));
	IDXGIAdapter * dxgiAdapter = nullptr;
	CHECK_HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter)));
	IDXGIFactory * dxgiFactory = nullptr;
	CHECK_HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)));
	//dxgiFactory->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
	CHECK_HR(dxgiFactory->CreateSwapChain(m_pD3D11Device, &swapChainDesc, &m_pSwapChain));
		ReleaseCOM(&dxgiDevice);
		ReleaseCOM(&dxgiAdapter);
		ReleaseCOM(&dxgiFactory);

	onResize();
}


CGameApp::~CGameApp()
{
	delete m_pGameWindow;
	ReleaseCOM(&m_pD3D11Device);
	ReleaseCOM(&m_pD3D11DeviceContext);
	ReleaseCOM(&m_pSwapChain);
	ReleaseCOM(&m_pRenderTargetView);
	ReleaseCOM(&m_pDepthStencilBuffer);
	ReleaseCOM(&m_pDepthStencilView);
}


void CGameApp::CalculateFrameStatus()
{
	
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if (m_GameTimer.TotalTime() - timeElapsed >= 1.0f) {
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_pGameWindow->GetHwnd(), outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
	
}

void CGameApp::Launch() 
{
	m_pGameWindow->StartWindow();

	m_GameTimer.Reset();

	MSG msg = { 0 };
	bool bIsRunning = true;
	const XMVECTORF32 BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };

	m_GameTimer.Start();

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CalculateFrameStatus();
			m_GameTimer.Tick();
			printf("%f \n", m_GameTimer.DeltaTime());

			m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView,
				reinterpret_cast<const float *>(&BLUE)
			);

			m_pD3D11DeviceContext->ClearDepthStencilView(
				m_pDepthStencilView,
				D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
			);

			m_pSwapChain->Present(0, 0);
		}
	}

	//m_GameTimer.Stop();
	std::cout << m_GameTimer.TotalTime() << std::endl;

}

void CGameApp::Update()
{

}

void CGameApp::Render()
{
	
}

void CGameApp::onResize()
{
	GAME_ASSERT(m_pD3D11DeviceContext, "D3D11Device is nullptr");
	GAME_ASSERT(m_pD3D11Device, "D3D11Device is nullptr");
	GAME_ASSERT(m_pSwapChain, "SwapChain is nullptr");

	ReleaseCOM(&m_pRenderTargetView);
	ReleaseCOM(&m_pDepthStencilView);
	ReleaseCOM(&m_pDepthStencilBuffer);

	CHECK_HR(m_pSwapChain->ResizeBuffers(1, m_pGameWindow->GetWidth(), m_pGameWindow->GetHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0))
	ID3D11Texture2D* dxgiTextureBuffer;
	CHECK_HR(m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&dxgiTextureBuffer)));
	CHECK_HR(m_pD3D11Device->CreateRenderTargetView(dxgiTextureBuffer, 0, &m_pRenderTargetView));
	ReleaseCOM(&dxgiTextureBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_pGameWindow->GetWidth();
	depthStencilDesc.Height = m_pGameWindow->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (m_MultiSampleQualityLevel) {
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
	}
	else {

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = NULL;
	depthStencilDesc.MiscFlags = NULL;

	CHECK_HR(m_pD3D11Device->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer));
	CHECK_HR(m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView));

	m_ViewportSettings.TopLeftX = 0.0f;
	m_ViewportSettings.TopLeftY = 0.0f;
	m_ViewportSettings.Width = static_cast<float>(m_pGameWindow->GetWidth());
	m_ViewportSettings.Height = static_cast<float>(m_pGameWindow->GetHeight());
	m_ViewportSettings.MinDepth = 0.0f;
	m_ViewportSettings.MaxDepth = 1.0f;

	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pD3D11DeviceContext->RSSetViewports(1, &m_ViewportSettings);

}