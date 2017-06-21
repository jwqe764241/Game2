#include <Sources/GameApp.h>

#define ENABLE_PRINT_ADAPTER_NAME

HRESULT hr;

//윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CGameApp * pGame = reinterpret_cast<CGameApp *>(GetWindowLongPtr(hWnd, 0));

	if (pGame) return pGame->MainProc(hWnd, msg, wParam, lParam);
	else return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK CGameApp::MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostMessage(hWnd, WM_DESTROY, 0L, 0L);
		}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

CGameApp::CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height)
	: m_pD3D11Device(nullptr), m_pD3D11DeviceContext(nullptr), m_pSwapChain(nullptr), m_vAdapters(),
	m_pRenderTargetView(nullptr), m_pDepthStencilBuffer(nullptr), m_pDepthStencilView(nullptr),
	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),m_SDKVersion(D3D11_SDK_VERSION), m_MultiSampleQualityLevel(NULL), m_FeatureLevel(),
	m_hInstance(hInstance), m_pstrFrameTitle(frameTitle), m_pstrWndClassName(wndClassName), m_iCmdShow(nCmdShow),
	m_sizeWindow({width, height})
{

	m_vAdapters.reserve(10);

	//윈도우 클래스 정의
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
		wndClass.cbSize			= sizeof(WNDCLASSEX);
		wndClass.style			= CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc	= WndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= sizeof(CGameApp*);
		wndClass.hInstance		= m_hInstance;
		wndClass.lpszMenuName	= nullptr;
		wndClass.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
		wndClass.lpszClassName	= m_pstrWndClassName;
		wndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClassEx(&wndClass);

	//윈도우 임시 셋팅 공간에 포인터 저장 --> 멤버 프로시저로 넘기기 위함

	//0일 경우 기본 크기로 지정
	if (width == 0 || height == 0) {
		m_sizeWindow = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}

	//어댑터 구해서 리스트에 저장
	IDXGIFactory * pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	UINT i = 0;
	IDXGIAdapter * pAdapter = nullptr;
	while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_vAdapters.push_back(pAdapter);
		++i;
	}
	ReleaseCOM(&pFactory); 


	//윈도우 생성
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_pstrWndClassName,
		m_pstrFrameTitle,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_sizeWindow.width,
		m_sizeWindow.height,
		NULL,
		NULL,
		m_hInstance,
		this);

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	//디버그 전용 플래그
	UINT createFlags = 0;
#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//DirectX 디바이스 생성
	//pAdapter != null 시 드라이버 타입은 -> D3D_DRIVER_TYPE_UNKNOWN
	D3D11CreateDevice(m_vAdapters.at(1),D3D_DRIVER_TYPE_UNKNOWN,0,createFlags,NULL,NULL,m_SDKVersion,&m_pD3D11Device,&m_FeatureLevel,&m_pD3D11DeviceContext);

	//에러 체크 -> 디바이스 생성 실패
	if (FAILED(hr)) {
		GAME_ASSERT(0 != 0, "Failed - D3D11CreateDevice");

		if (MessageBox(NULL, L"Failed - D3D11CreateDevice", L"ERROR!!", MB_OK) == IDOK) {
			if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
				PostQuitMessage(0);
			}
		}
	}
	
	//에러 체크 -> D11 지원 안함
	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0) {
		GAME_ASSERT(0 != 0, "Failed - Not Support Directx 11");

		if (MessageBox(NULL, L"Failed - Not Support Directx 11", L"Error!!", MB_OK) == IDOK) {
			if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
				PostQuitMessage(0);
			}
		}
	}

	//멀티 샘플 레벨 체크
	if (m_pD3D11Device != nullptr) {
		CHECK_HR(m_pD3D11Device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MultiSampleQualityLevel
		));
		GAME_ASSERT(m_MultiSampleQualityLevel > 0, "Unexpected ERROR");
	}


	//스왑 체인 정의
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width					 = m_sizeWindow.width;
	swapChainDesc.BufferDesc.Height					 = m_sizeWindow.height;
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
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	

	//스왑 체인 생성
	IDXGIDevice * dxgiDevice = nullptr;
	m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	IDXGIAdapter * dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
	IDXGIFactory * dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
	//dxgiFactory->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
	dxgiFactory->CreateSwapChain(m_pD3D11Device, &swapChainDesc, &m_pSwapChain);
	//dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		ReleaseCOM(&dxgiDevice);
		ReleaseCOM(&dxgiAdapter);
		ReleaseCOM(&dxgiFactory);

	m_pD3D11DeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

	onResize();
}

CGameApp::~CGameApp()
{
	UnregisterClass(m_pstrWndClassName, m_hInstance);
	CloseWindow(m_hWnd);
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
		outs << m_pstrFrameTitle << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_hWnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void CGameApp::Launch() 
{
	onShowWindow();

	m_GameTimer.Reset();

	MSG msg = { 0 };
	bool bIsRunning = true;

	m_GameTimer.Start();

	//디버그 모드에다 어댑터 이름 출력 온일 때 출력해줌
#if defined(_DEBUG) && defined(ENABLE_PRINT_ADAPTER_NAME)
	for (IDXGIAdapter * adapter : m_vAdapters) {
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);
		wprintf(L"%s \n", adapterDesc.Description);
	}
#endif

	//메시지 루프
	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Update();
		Render();

		m_pSwapChain->Present(0, 0);
	}

	//std::cout << m_GameTimer.TotalTime() << std::endl;
}


void CGameApp::Update()
{
	m_GameTimer.Tick();
	CalculateFrameStatus();
}


void CGameApp::Render()
{
	static const float * RED	= reinterpret_cast<const float *>(&CUSTOM_COLOR::RED);
	static const float * GREEN	= reinterpret_cast<const float *>(&CUSTOM_COLOR::GREEN);
	static const float * BLUE	= reinterpret_cast<const float *>(&CUSTOM_COLOR::BLUE);
	static const float * BLACK = reinterpret_cast<const float *>(&CUSTOM_COLOR::BLACK);

	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView,
		BLACK
	);

	m_pD3D11DeviceContext->ClearDepthStencilView(
		m_pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
	);
}


void CGameApp::onResize()
{
	GAME_ASSERT(m_pD3D11DeviceContext, "D3D11Device is nullptr");
	GAME_ASSERT(m_pD3D11Device, "D3D11Device is nullptr");
	GAME_ASSERT(m_pSwapChain, "SwapChain is nullptr");

	//이전에 사용하던것 해제
	ReleaseCOM(&m_pRenderTargetView);
	ReleaseCOM(&m_pDepthStencilView);
	ReleaseCOM(&m_pDepthStencilBuffer);

	//버퍼 리사이즈 후 렌더타겟뷰 재생성
	m_pSwapChain->ResizeBuffers(1, m_sizeWindow.width, m_sizeWindow.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* dxgiTextureBuffer = nullptr;
	m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&dxgiTextureBuffer));
	m_pD3D11Device->CreateRenderTargetView(dxgiTextureBuffer, 0, &m_pRenderTargetView);
	ReleaseCOM(&dxgiTextureBuffer);

	//스텐실 버퍼 생성
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_sizeWindow.width;
	depthStencilDesc.Height = m_sizeWindow.height;
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

	//버퍼 생성 빛 버퍼뷰 생성
	m_pD3D11Device->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
	m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);

	//버퍼와 버퍼뷰 설정
	m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//뷰포트 셋팅
	m_ViewportSettings.TopLeftX = 0.0f;
	m_ViewportSettings.TopLeftY = 0.0f;
	m_ViewportSettings.Width  = static_cast<float>(m_sizeWindow.width);
	m_ViewportSettings.Height = static_cast<float>(m_sizeWindow.height);
	m_ViewportSettings.MinDepth = 0.0f;
	m_ViewportSettings.MaxDepth = 1.0f;

	//뷰표트 설정
	m_pD3D11DeviceContext->RSSetViewports(1, &m_ViewportSettings);
}

void CGameApp::onShowWindow()
{
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	ShowCursor(false);
	UpdateWindow(m_hWnd);
}