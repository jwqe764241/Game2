#include <Sources/GameApp.h>

#define ENABLE_PRINT_ADAPTER_NAME

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
	case WM_SIZE:
		onResize();
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

CGameApp::CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height)
	: m_AppInfo({nullptr}), m_vAdapters(),
	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),m_SDKVersion(D3D11_SDK_VERSION), m_MultiSampleQualityLevel(NULL), m_FeatureLevel(),
	m_hInstance(hInstance), m_pstrFrameTitle(frameTitle), m_pstrWndClassName(wndClassName), m_iCmdShow(nCmdShow)
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
	else {
		m_sizeWindow.width = width;
		m_sizeWindow.height = height;
	}

	//윈도우 생성
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW ,
		m_pstrWndClassName,
		m_pstrFrameTitle,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_sizeWindow.width,
		m_sizeWindow.height,
		NULL,
		NULL,
		m_hInstance,
		this);

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	//-- HRESULT 체킹 안함 --//
	//팩토리 생성
	IDXGIFactory* l_pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&l_pFactory));
	
	//어댑터 나열
	IDXGIAdapter*  l_pAdapter = nullptr;
	std::vector<IDXGIAdapter *> l_pAdapterList;

	//어댑터 벡터에 푸쉬
	for (int i = 0; l_pFactory->EnumAdapters(i, &l_pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		l_pAdapterList.push_back(l_pAdapter);
	}

	IDXGIOutput* l_pAdapterOutput = nullptr;
	l_pAdapterList.at(0)->EnumOutputs(NULL, &l_pAdapterOutput);

	//리스트 갯수 얻기 위함
	unsigned int l_uiModes = 0;
	l_pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, NULL);

	//갯수대로 생성 후, 속성 얻기
	DXGI_MODE_DESC * l_pModeList = new DXGI_MODE_DESC[l_uiModes];
	l_pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, l_pModeList);

	//새로고침 비율 분자 분모 얻기
	//60hrz -> msi 노트북
	int l_iNumerator, l_iDenominator;
	for (int i = 0; i < l_uiModes; ++i) {
		if ((l_pModeList[i].Width == m_sizeWindow.width) &&
			(l_pModeList[i].Height == m_sizeWindow.height)) {
			l_iNumerator	= l_pModeList[i].RefreshRate.Numerator;
			l_iDenominator	= l_pModeList[i].RefreshRate.Denominator;
		}
	}

	//어댑터 데이터 벡터에 추가
	for (int i = 0; i < l_pAdapterList.size(); ++i){
		DXGI_ADAPTER_DESC l_pAdapterDesc;
		l_pAdapterList[i]->GetDesc(&l_pAdapterDesc);

		ADAPTERINFO adapterInfo;
		adapterInfo.Description = l_pAdapterDesc.Description;
		adapterInfo.ID          = l_pAdapterDesc.AdapterLuid;

		m_AdapterInfoList.push_back(adapterInfo);
	}

	//해제
	delete[] l_pModeList;
	ReleaseCOM(&l_pAdapterOutput);
	for (IDXGIAdapter* adapter : l_pAdapterList)
		ReleaseCOM(&adapter);
	ReleaseCOM(&l_pFactory);

	//스왑체인 설정
	DXGI_SWAP_CHAIN_DESC swapChainDesc; 
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width	                 = m_sizeWindow.width;
	swapChainDesc.BufferDesc.Height                  = m_sizeWindow.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator   = l_iNumerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = l_iDenominator;
	swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	if (m_MultiSampleQualityLevel) {
		swapChainDesc.SampleDesc.Count   = 4;
		swapChainDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
	}
	else {
		swapChainDesc.SampleDesc.Count   = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	swapChainDesc.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount  = 1;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed     = true;
	swapChainDesc.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags        = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT createFlags = 0;
#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, createFlags, &featureLevel, 1, D3D11_SDK_VERSION, 
		&swapChainDesc, &m_AppInfo.pSwapChain, &m_AppInfo.pD3D11Device, NULL, &m_AppInfo.pD3D11DeviceContext);
	m_AppInfo.pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_AppInfo.pBackBuffer));
	m_AppInfo.pD3D11Device->CreateRenderTargetView(m_AppInfo.pBackBuffer, NULL, &m_AppInfo.pRenderTargetView);

	onResize();

	/*
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
		ReleaseCOM(&dxgiFactory);;

	m_pD3D11DeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	); 

#ifdef TEST_RENDER_BOX
	BuildBox();
#endif

	onResize();
	*/
}

CGameApp::~CGameApp()
{
	UnregisterClass(m_pstrWndClassName, m_hInstance);
	CloseWindow(m_hWnd);
	ReleaseCOM(&m_AppInfo.pD3D11Device);
	ReleaseCOM(&m_AppInfo.pD3D11DeviceContext);
	ReleaseCOM(&m_AppInfo.pSwapChain);
	ReleaseCOM(&m_AppInfo.pRenderTargetView);
	ReleaseCOM(&m_AppInfo.pBackBuffer);
	ReleaseCOM(&m_AppInfo.pDepthStencilView);
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
		outs << m_pstrFrameTitle << L"  Adapter: "<< m_AdapterInfoList[1].Description << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_hWnd, outs.str().c_str());

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void CGameApp::LoadAssets() 
{

}

void CGameApp::Launch() 
{
	onShowWindow();

	m_GameTimer.Reset();
	m_GameTimer.Start();

	MSG msg = { 0 };
	bool bIsRunning = true;

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

		m_AppInfo.pSwapChain->Present(0, 0);
	}
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
	static const float * BLACK  = reinterpret_cast<const float *>(&CUSTOM_COLOR::BLACK);

	m_AppInfo.pD3D11DeviceContext->ClearRenderTargetView(m_AppInfo.pRenderTargetView,
		BLACK
	);

	m_AppInfo.pD3D11DeviceContext->ClearDepthStencilView(
		m_AppInfo.pDepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0
	);
}


void CGameApp::onResize()
{
	GAME_ASSERT(m_AppInfo.pD3D11DeviceContext, "D3D11Device is nullptr");
	GAME_ASSERT(m_AppInfo.pD3D11Device, "D3D11Device is nullptr");
	GAME_ASSERT(m_AppInfo.pSwapChain, "SwapChain is nullptr");

	//이전에 사용하던것 해제
	ReleaseCOM(&m_AppInfo.pRenderTargetView);
	ReleaseCOM(&m_AppInfo.pDepthStencilView);
	ReleaseCOM(&m_AppInfo.pBackBuffer);

	RECT rect; GetWindowRect(m_hWnd, &rect);
	m_sizeWindow.width  = rect.right - rect.left;
	m_sizeWindow.height = rect.bottom - rect.top;

	//버퍼 리사이즈 후 렌더타겟뷰 재생성
	m_AppInfo.pSwapChain->ResizeBuffers(1, m_sizeWindow.width, m_sizeWindow.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* dxgiTextureBuffer = nullptr;
	m_AppInfo.pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&dxgiTextureBuffer));
	m_AppInfo.pD3D11Device->CreateRenderTargetView(dxgiTextureBuffer, 0, &m_AppInfo.pRenderTargetView);
	ReleaseCOM(&dxgiTextureBuffer);

	//스텐실 버퍼 생성
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width     = m_sizeWindow.width;
	depthBufferDesc.Height    = m_sizeWindow.height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (m_MultiSampleQualityLevel) {
		depthBufferDesc.SampleDesc.Count   = 4;
		depthBufferDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
	}
	else {
		depthBufferDesc.SampleDesc.Count   = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
	}
	depthBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = NULL;
	depthBufferDesc.MiscFlags      = NULL;
	//버퍼 생성
	m_AppInfo.pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_AppInfo.pBackBuffer);
	
	//스텐실 생성
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable    = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;
///
	depthStencilDesc.StencilEnable    = true;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.StencilReadMask  = 0xFF;
///
	depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
///
	depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
///
	m_AppInfo.pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_AppInfo.pDepthStencilState);
	m_AppInfo.pD3D11DeviceContext->OMSetDepthStencilState(m_AppInfo.pDepthStencilState, 1);

	//깊이 스텐실 뷰 생성
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	m_AppInfo.pD3D11Device->CreateDepthStencilView(m_AppInfo.pBackBuffer, &depthStencilViewDesc, &m_AppInfo.pDepthStencilView);
	
	//버퍼와 버퍼뷰 설정
	m_AppInfo.pD3D11DeviceContext->OMSetRenderTargets(1, &m_AppInfo.pRenderTargetView, m_AppInfo.pDepthStencilView);

	//뷰포트 셋팅
	D3D11_VIEWPORT viewportSettings;
	viewportSettings.TopLeftX = 0.0f;
	viewportSettings.TopLeftY = 0.0f;
	viewportSettings.Width    = static_cast<float>(m_sizeWindow.width);
	viewportSettings.Height   = static_cast<float>(m_sizeWindow.height);
	viewportSettings.MinDepth = 0.0f;
	viewportSettings.MaxDepth = 1.0f;

	//뷰표트 설정
	m_AppInfo.pD3D11DeviceContext->RSSetViewports(1, &viewportSettings);
}

void CGameApp::onShowWindow()
{
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	ShowCursor(true);
	UpdateWindow(m_hWnd);
}

//void CGameApp::BuildShader()
//{
//
//}
//
//void CGameApp::BuildVertexLayout()
//{
//
//}
//
//#ifdef TEST_RENDER_BOX
//void CGameApp::BuildBox() 
//{
//	//정점 데이터 설정
//	Vertex::Vertex1 vertices[] = {
//		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::BLACK)	},
//		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::BLUE)	},
//		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::CYAN)	},
//		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::GREEN)	},
//		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::MAGENTA) },
//		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::RED)		},
//		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::WHITE)	},
//		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::YELLOW)	},
//	};
//
//	//정점 버퍼 설정 서술
//	D3D11_BUFFER_DESC vbd;
//	vbd.Usage = D3D11_USAGE_IMMUTABLE;
//	vbd.ByteWidth = sizeof(Vertex::Vertex1) * 8;
//	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vbd.CPUAccessFlags = 0;
//	vbd.MiscFlags = 0;
//	vbd.StructureByteStride = 0;
//	//정점 버퍼 서브소스 서술
//	D3D11_SUBRESOURCE_DATA vinitdata;
//	vinitdata.pSysMem = vertices;
//	//정점 버퍼 생성
//	m_pD3D11Device->CreateBuffer(&vbd, &vinitdata, &m_pBoxVertexBuffer);
//
//	
//	//색인 데이터 설정
//	UINT indices[] = {
//		0, 1, 2,
//		0, 2, 3,
//
//		4, 6, 5,
//		4, 7, 6,
//
//		4, 5, 1,
//		4, 1, 0,
//
//		3, 2, 6,
//		3, 6, 7,
//
//		1, 5, 6,
//		1, 6, 2,
//
//		4, 0, 3,
//		4, 3, 7
//	};
//
//	//색인 버퍼 설정 서술
//	D3D11_BUFFER_DESC ibd;
//	ibd.Usage = D3D11_USAGE_IMMUTABLE;
//	ibd.ByteWidth = sizeof(UINT) * 36;
//	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	ibd.CPUAccessFlags = 0;
//	ibd.MiscFlags = 0;
//	ibd.StructureByteStride = 0;
//	//색인 버퍼 서브소스 서술
//	D3D11_SUBRESOURCE_DATA iinitdata;
//	iinitdata.pSysMem = indices;
//	//색인 버퍼 생성
//	m_pD3D11Device->CreateBuffer(&ibd, &iinitdata, &m_pBoxIndexBuffer);
//}
//#endif