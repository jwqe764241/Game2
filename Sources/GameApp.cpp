#include <Sources/GameApp.h>

////윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		std::cout << "FDgsdf" << std::endl;
		CGameApp::GetInstance().onResize();
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

CGameApp::CGameApp()
	: m_AppInfo({nullptr}), m_vAdapters(), m_screenDepth(0.0f), m_screenNear(0.0f),
	m_hInstance(NULL), m_FrameTitle(nullptr), m_WndClassName(nullptr), m_CmdShow(NULL)
{
}

CGameApp::~CGameApp()
{
}

bool CGameApp::Initialize(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height, float screenDepth, float screenNear)
{
	m_vAdapters.reserve(10);

	m_hInstance = hInstance;
	m_FrameTitle = frameTitle;
	m_WndClassName = wndClassName;
	m_CmdShow = nCmdShow;
	m_screenDepth = screenDepth;
	m_screenNear = screenNear;

	//윈도우 클래스 정의 및 등록
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = 0;
		wndClass.hInstance = m_hInstance;
		wndClass.lpszMenuName = nullptr;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.lpszClassName = m_WndClassName;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClassEx(&wndClass);

	//폭, 너비가 0일 때 현재 해상도 값으로 변경
	if (width == 0 || height == 0)
	{
		m_WindowSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
	else
	{
		m_WindowSize.width = width;
		m_WindowSize.height = height;
	}

	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		m_WndClassName,
		m_FrameTitle,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_WindowSize.width,
		m_WindowSize.height,
		NULL,
		NULL,
		m_hInstance,
		this);

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(this));


	//팩토리 생성
	IDXGIFactory* l_pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&l_pFactory));

	//어댑터 나열
	IDXGIAdapter* l_pAdapter = nullptr;
	IDXGIOutput * l_pOutput = nullptr;

	for (int i = 0; l_pFactory->EnumAdapters(i, &l_pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		m_vAdapters.push_back(l_pAdapter);
	}

	std::cout << "-----------------------인식된 어댑터 정보-----------------------" << std::endl;
	for (int i = 0; i < m_vAdapters.size(); i++)
	{
		DXGI_ADAPTER_DESC desc;
		m_vAdapters[i]->GetDesc(&desc);

		std::wcout << "\nGPU : " << desc.Description << std::endl;
		std::cout << "DedicatedVideoMemory  : " << desc.DedicatedVideoMemory << std::endl;
		std::cout << "DedicatedSystemMemory : " << desc.DedicatedSystemMemory << std::endl;
		std::cout << "SharedSystemMemory	: " << desc.SharedSystemMemory << "\n" << std::endl;

		int iOutput = 0;
		while (m_vAdapters[i]->EnumOutputs(iOutput++, &l_pOutput) != DXGI_ERROR_NOT_FOUND) {
			m_vAdaptersOutputs.push_back(l_pOutput);
		}

	}
	std::cout << "----------------------------------------------------------------" << std::endl;


	//디스플레이 모드 갯수 가져오 위함
	unsigned int l_uiModes = 0;
	m_vAdaptersOutputs[0]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, NULL);
	//디스플레이 모드 전부 가져옴
	DXGI_MODE_DESC * l_pModeList = new DXGI_MODE_DESC[l_uiModes];
	m_vAdaptersOutputs[0]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, l_pModeList);

	std::cout << "-----------------------가능한 디스플레이 모드-----------------------" << std::endl;
	for (int i = 0; i < l_uiModes; i++)
	{
		std::cout << "\n" << i << ". 해상도 (width X height)" << l_pModeList[i].Width << "X" << l_pModeList[i].Height << std::endl;
		std::cout << "픽셀 포맷 : " << l_pModeList[i].Format << std::endl;
		std::cout << "보고율 : " << l_pModeList[i].RefreshRate.Denominator << "/" << l_pModeList[i].RefreshRate.Numerator << std::endl;
	}
	std::cout << "----------------------------------------------------------------" << std::endl;


	//현재 윈도우와 맞는 디스플레이 모드에서 보고율 구하기
	int l_iNumerator, l_iDenominator;
	for (int i = 0; i < l_uiModes; ++i)
	{
		if ((l_pModeList[i].Width == m_WindowSize.width) && (l_pModeList[i].Height == m_WindowSize.height))
		{
			l_iNumerator = l_pModeList[i].RefreshRate.Numerator;
			l_iDenominator = l_pModeList[i].RefreshRate.Denominator;
		}
	}

	//디스플레이 모드 리스트 추가
	for (int i = 0; i < m_vAdapters.size(); ++i)
	{
		DXGI_ADAPTER_DESC l_pAdapterDesc;
		m_vAdapters[i]->GetDesc(&l_pAdapterDesc);

		AdapterInfo adapterInfo;
		adapterInfo.Description = l_pAdapterDesc.Description;
		adapterInfo.ID = l_pAdapterDesc.AdapterLuid;

		m_vAdapterInfoList.push_back(adapterInfo);
	}


	delete[] l_pModeList;
	Utils::Release(&l_pFactory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = m_WindowSize.width;
	swapChainDesc.BufferDesc.Height = m_WindowSize.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = l_iNumerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = l_iDenominator;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (m_MultiSampleQualityLevel)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &m_AppInfo.pSwapChain, &m_AppInfo.pD3D11Device, NULL, &m_AppInfo.pD3D11DeviceContext);
	m_AppInfo.pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_AppInfo.pBackBuffer));
	m_AppInfo.pD3D11Device->CreateRenderTargetView(m_AppInfo.pBackBuffer, NULL, &m_AppInfo.pRenderTargetView);

	onResize();

	return true;
}

void CGameApp::Release()
{
	UnregisterClass(m_WndClassName, m_hInstance);
	//CloseWindow(m_hWnd);
	DestroyWindow(m_hWnd);
	Utils::Release(&m_AppInfo.pRasterizeState);
	Utils::Release(&m_AppInfo.pDepthStencilView);
	Utils::Release(&m_AppInfo.pDepthDisableStencilState);
	Utils::Release(&m_AppInfo.pDepthStencilState);
	Utils::Release(&m_AppInfo.pBackBuffer);
	Utils::Release(&m_AppInfo.pRenderTargetView);
	Utils::Release(&m_AppInfo.pSwapChain);
	Utils::Release(&m_AppInfo.pD3D11DeviceContext);
	Utils::Release(&m_AppInfo.pD3D11Device);

	for (IDXGIAdapter* adapter : m_vAdapters)
	{
		Utils::Release(&adapter);
	}

	for (IDXGIOutput* output : m_vAdaptersOutputs)
	{
		Utils::Release(&output);
	}
}

void CGameApp::CalculateFrameStatus()
{
	static int frameCnt = 0;	//프레임 카운트
	static float timeElapsed = 0.0f;	//흐른 시간

	frameCnt++; 

	//1초가 지났을 때 프레임 정보 업데이트
	if (m_GameTimer.TotalTime() - timeElapsed >= 1.0f) 
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;
	
		std::wostringstream outs;
		outs.precision(6);
		outs << m_FrameTitle << L"  Adapter: "<< m_vAdapterInfoList[1].Description << L"    "
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
	LoadAssets();
	onShowWindow();

	if (!GameInput::GetInstance().Initialize(m_hInstance, m_hWnd, m_WindowSize.width, m_WindowSize.height))
	{
		MessageBox(m_hWnd, L"Error!!", L"Cannot Initialize Input", MB_OK);
		return;
	}

	CGameLevelLoader::GetInstance().LoadLevel(new TestLevel1());

	m_GameTimer.Reset();
	m_GameTimer.Start();

	MSG msg = { 0 };
	bool bIsRunning = true;

	//메시지 루프 시작
	while (msg.message != WM_QUIT) 
	{

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (GameInput::GetInstance().IsEscapeProcessed())
		{
			return;
		}
		m_AppInfo.pD3D11DeviceContext->ClearRenderTargetView(m_AppInfo.pRenderTargetView, GameColors::GREEN);

		m_AppInfo.pD3D11DeviceContext->ClearDepthStencilView(m_AppInfo.pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		Update();
		if (!Render())
		{
			//Error...
		}

		m_AppInfo.pSwapChain->Present(0, 0);
	}
}


void CGameApp::Update()
{
	m_GameTimer.Tick();
	GameInput::GetInstance().Frame();
	CGameLevelLoader::GetInstance().UpdateLevel(m_GameTimer.DeltaTime());
	CalculateFrameStatus();
}


bool CGameApp::Render()
{
	TurnOffZBuffer();

	m_Camera.Render();

	//CGameAssetLoader::GetInstance().GetAsset("TestAsset")->Render(m_AppInfo.pD3D11DeviceContext, 20, 20);

	//if (!TextureShader::GetInstance().Render(m_AppInfo.pD3D11DeviceContext, CGameAssetLoader::GetInstance().GetAsset("TestAsset")->GetIndexCount(),
	//	m_Matrix.worldMatrix, m_Camera.GetViewMatrix(), m_Matrix.orthMatrix, CGameAssetLoader::GetInstance().GetAsset("TestAsset")->GetTexture()))
	//{
	//	return false;
	//}

	CGameLevelLoader::GetInstance().RenderLevel(m_AppInfo.pD3D11DeviceContext);

	TurnOnZBuffer();

	return true;
}


void CGameApp::onResize()
{
	GAME_ASSERT(m_AppInfo.pD3D11DeviceContext, "D3D11Device is nullptr");
	GAME_ASSERT(m_AppInfo.pD3D11Device, "D3D11Device is nullptr");
	GAME_ASSERT(m_AppInfo.pSwapChain, "SwapChain is nullptr");

	//재할당 전 해제
	Utils::Release(&m_AppInfo.pRenderTargetView);
	Utils::Release(&m_AppInfo.pDepthStencilView);
	Utils::Release(&m_AppInfo.pBackBuffer);

	RECT rect; GetWindowRect(m_hWnd, &rect);
	m_WindowSize.width  = rect.right - rect.left;
	m_WindowSize.height = rect.bottom - rect.top;

	//스왑체인 버퍼 사이즈 재설정
	m_AppInfo.pSwapChain->ResizeBuffers(1, m_WindowSize.width, m_WindowSize.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* dxgiTextureBuffer = nullptr;
	m_AppInfo.pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&dxgiTextureBuffer));
	m_AppInfo.pD3D11Device->CreateRenderTargetView(dxgiTextureBuffer, 0, &m_AppInfo.pRenderTargetView);
	Utils::Release(&dxgiTextureBuffer);

	//깊이 버퍼 재설정
	D3D11_TEXTURE2D_DESC depthBufferDesc;
		depthBufferDesc.Width     = m_WindowSize.width;
		depthBufferDesc.Height    = m_WindowSize.height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (m_MultiSampleQualityLevel) 
		{
			depthBufferDesc.SampleDesc.Count   = 4;
			depthBufferDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
		}
		else 
		{
			depthBufferDesc.SampleDesc.Count   = 1;
			depthBufferDesc.SampleDesc.Quality = 0;
		}
		depthBufferDesc.Usage          = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = NULL;
		depthBufferDesc.MiscFlags      = NULL;
	m_AppInfo.pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_AppInfo.pBackBuffer);
	
	//
	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.IndependentBlendEnable = FALSE;
		blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
		blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_AppInfo.pD3D11Device->CreateBlendState(&blendStateDesc, &m_AppInfo.pBlendState);
	m_AppInfo.pD3D11DeviceContext->OMSetBlendState(m_AppInfo.pBlendState, NULL, 0xFFFFFF);


	//깊이 스텐실 버퍼 생성 -> Z버퍼 켬
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable    = true;
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
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
	m_AppInfo.pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_AppInfo.pDepthStencilState);

	//깊이 스텐실 버퍼 생성 -> Z버퍼 끔
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		///
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		///
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		///
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	m_AppInfo.pD3D11Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_AppInfo.pDepthDisableStencilState);
	//기본 Z버퍼 사용 버퍼로 설정
	m_AppInfo.pD3D11DeviceContext->OMSetDepthStencilState(m_AppInfo.pDepthStencilState, 1);

	//깊이 스텐실 뷰 생성 및 설정
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	m_AppInfo.pD3D11Device->CreateDepthStencilView(m_AppInfo.pBackBuffer, &depthStencilViewDesc, &m_AppInfo.pDepthStencilView);
	m_AppInfo.pD3D11DeviceContext->OMSetRenderTargets(1, &m_AppInfo.pRenderTargetView, m_AppInfo.pDepthStencilView);

	//레스터라이저 설정
	D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;
	m_AppInfo.pD3D11Device->CreateRasterizerState(&rasterDesc, &m_AppInfo.pRasterizeState);
	m_AppInfo.pD3D11DeviceContext->RSSetState(m_AppInfo.pRasterizeState);

	//뷰포트 설정
	D3D11_VIEWPORT viewportSettings;
		viewportSettings.TopLeftX = 0.0f;
		viewportSettings.TopLeftY = 0.0f;
		viewportSettings.Width    = static_cast<float>(m_WindowSize.width);
		viewportSettings.Height   = static_cast<float>(m_WindowSize.height);
		viewportSettings.MinDepth = 0.0f;
		viewportSettings.MaxDepth = 1.0f;
	m_AppInfo.pD3D11DeviceContext->RSSetViewports(1, &viewportSettings);

	float fieldOfView  = static_cast<float>(D3DX_PI) / 4.0f;
	float screenAspect = static_cast<float>(m_WindowSize.width) / static_cast<float>(m_WindowSize.height);

	D3DXMatrixPerspectiveFovLH(&m_Matrix.projectionMatrix, fieldOfView, screenAspect, m_screenNear, m_screenDepth);
	D3DXMatrixIdentity(&m_Matrix.worldMatrix);
	D3DXMatrixOrthoLH(&m_Matrix.orthMatrix, static_cast<float>(m_WindowSize.width), static_cast<float>(m_WindowSize.height), m_screenNear, m_screenDepth);

	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);
	if (!TextureShader::GetInstance().Initialize(m_AppInfo.pD3D11Device, m_hWnd))
	{
		MessageBox(m_hWnd, L"Error!!", L"Cannot Initialize Texture Shaders!", MB_OK);
		return;
	}

	CGameAssetLoader::GetInstance().Initialize(m_AppInfo.pD3D11Device, m_AppInfo.pD3D11DeviceContext, &m_WindowSize.width, &m_WindowSize.height);
}

void CGameApp::onShowWindow()
{
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	UpdateWindow(m_hWnd);
}

void CGameApp::TurnOnZBuffer()
{
	m_AppInfo.pD3D11DeviceContext->OMSetDepthStencilState(m_AppInfo.pDepthStencilState, 1);
}

void CGameApp::TurnOffZBuffer()
{
	m_AppInfo.pD3D11DeviceContext->OMSetDepthStencilState(m_AppInfo.pDepthDisableStencilState, 1);
}

CGameApp& CGameApp::GetInstance()
{
	static CGameApp Instance;

	return Instance;
}

D3DXMATRIX& CGameApp::GetProjectionMatrix()
{
	return m_Matrix.projectionMatrix;
}

D3DXMATRIX& CGameApp::GetWorldMatrix()
{
	return m_Matrix.worldMatrix;
}

D3DXMATRIX& CGameApp::GetorthogonalMatrix()
{
	return m_Matrix.orthMatrix;
}