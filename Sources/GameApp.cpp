#include <Sources/GameApp.h>

////윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
	case WM_SIZE:
		{
			CGameApp::GetInstance().onResize();
			break;
		}
	case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_MAXIMIZE:
				{
					CGameApp::GetInstance().GetAppInfo()->pSwapChain->SetFullscreenState(true, nullptr);
					CGameApp::GetInstance().onResize();
					break;
				}
			case SC_CLOSE:
				PostQuitMessage(0);
				break;
			}
			break;
		}
	/*
		GameInput 전용 메시지 처리기
	*/
	case WM_KEYDOWN:
		{
			if (wParam == VK_F12)
			{
				int result = NULL;
				CGameApp::GetInstance().GetAppInfo()->pSwapChain->GetFullscreenState(&result, nullptr);

				if (result == TRUE)
				{
					CGameApp::GetInstance().GetAppInfo()->pSwapChain->SetFullscreenState(false, nullptr);
				}
				break;
			}
			GameInput2::GetInstance().PressKey(wParam);
			break;
		}
	case WM_KEYUP:
		{
			GameInput2::GetInstance().ReleaseKey(wParam);
			break;
		}
	case WM_MOUSEMOVE:
		{
			GameInput2::GetInstance().UpdateMousePosition(POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
			break;
		}
	//왼쪽 마우스 메시지 처리기
	case WM_LBUTTONDOWN:
		{
			GameInput2::GetInstance().PressKey(VK_LBUTTON);
			break;
		}
	case WM_LBUTTONUP:
		{
			GameInput2::GetInstance().ReleaseKey(VK_LBUTTON);
			break;
		}
	//오른쪽 마우스 메시지 처리기
	case WM_RBUTTONDOWN:
		{
			GameInput2::GetInstance().PressKey(VK_RBUTTON);
			break;
		}
	case WM_RBUTTONUP:
		{
			GameInput2::GetInstance().ReleaseKey(VK_RBUTTON);
			break;
		}
	//가운데 마우스 메시지 처리기
	case WM_MBUTTONDOWN:
		{
			GameInput2::GetInstance().PressKey(VK_MBUTTON);
			break;
		}
	case WM_MBUTTONUP:
		{
			GameInput2::GetInstance().ReleaseKey(VK_MBUTTON);
			break;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

CGameApp::CGameApp()
	: AppComponents({nullptr}), AdaptersList(), ScreenDepth(0.0f), ScreenNear(0.0f),
	HInstance(NULL), FrameTitle(nullptr), WndClassName(nullptr), CmdShow(NULL)
{
}

CGameApp::~CGameApp()
{
	Release();
}

bool CGameApp::Initialize(HINSTANCE hInstance, wchar_t* frameTitle, wchar_t* wndClassName, int nCmdShow, int width, int height, float screenDepth, float screenNear)
{
	AdaptersList.reserve(10);

	HInstance = hInstance;
	FrameTitle = frameTitle;
	WndClassName = wndClassName;
	CmdShow = nCmdShow;
	ScreenDepth = screenDepth;
	ScreenNear = screenNear;

	//윈도우 클래스 정의 및 등록
	WNDCLASSEX wndClass;
	ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.cbClsExtra = 0;
		wndClass.hInstance = HInstance;
		wndClass.lpszMenuName = nullptr;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.lpszClassName = WndClassName;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClassEx(&wndClass);

	//폭, 너비가 0일 때 현재 해상도 값으로 변경
	if (width == 0 || height == 0)
	{
		Windowsize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
	else
	{
		Windowsize.width = width;
		Windowsize.height = height;
	}

	HWnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		WndClassName,
		FrameTitle,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		Windowsize.width,
		Windowsize.height,
		NULL,
		NULL,
		HInstance,
		this);

	SetWindowLongPtr(HWnd, 0, reinterpret_cast<LONG_PTR>(this));


	//팩토리 생성
	IDXGIFactory* l_pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&l_pFactory));

	//어댑터 나열
	IDXGIAdapter* l_pAdapter = nullptr;
	IDXGIOutput*  l_pOutput = nullptr;

	for (int i = 0; l_pFactory->EnumAdapters(i, &l_pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		AdaptersList.push_back(l_pAdapter);
	}

	std::cout << "-----------------------인식된 어댑터 정보-----------------------" << std::endl;
	for (int i = 0; i < AdaptersList.size(); i++)
	{
		DXGI_ADAPTER_DESC desc;
		AdaptersList[i]->GetDesc(&desc);

		std::wcout << "\nGPU : " << desc.Description << std::endl;
		std::cout << "DedicatedVideoMemory  : " << desc.DedicatedVideoMemory << std::endl;
		std::cout << "DedicatedSystemMemory : " << desc.DedicatedSystemMemory << std::endl;
		std::cout << "SharedSystemMemory	: " << desc.SharedSystemMemory << "\n" << std::endl;

		int iOutput = 0;
		while (AdaptersList[i]->EnumOutputs(iOutput++, &l_pOutput) != DXGI_ERROR_NOT_FOUND) {
			AdaptersOutputsList.push_back(l_pOutput);
		}

	}
	std::cout << "----------------------------------------------------------------" << std::endl;


	//디스플레이 모드 갯수 가져오 위함
	unsigned int l_uiModes = 0;
	AdaptersOutputsList[0]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, NULL);
	//디스플레이 모드 전부 가져옴
	DXGI_MODE_DESC* l_pModeList = new DXGI_MODE_DESC[l_uiModes];
	ZeroMemory(l_pModeList, sizeof(DXGI_MODE_DESC) * l_uiModes);
	AdaptersOutputsList[0]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, l_pModeList);

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
		if ((l_pModeList[i].Width == Windowsize.width) && (l_pModeList[i].Height == Windowsize.height))
		{
			l_iNumerator = l_pModeList[i].RefreshRate.Numerator;
			l_iDenominator = l_pModeList[i].RefreshRate.Denominator;
		}
	}

	//디스플레이 모드 리스트 추가
	for (int i = 0; i < AdaptersList.size(); ++i)
	{
		DXGI_ADAPTER_DESC l_pAdapterDesc;
		AdaptersList[i]->GetDesc(&l_pAdapterDesc);

		AdapterInfo adapterInfo;
		adapterInfo.Description = l_pAdapterDesc.Description;
		adapterInfo.ID = l_pAdapterDesc.AdapterLuid;

		AdapterInfoList.push_back(adapterInfo);
	}


	delete[] l_pModeList;
	Utils::Release(&l_pFactory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = Windowsize.width;
	swapChainDesc.BufferDesc.Height = Windowsize.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = l_iNumerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = l_iDenominator;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (MultiSampleQualityLevel)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = MultiSampleQualityLevel - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = HWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, 0, 0, D3D10_SDK_VERSION,
		&swapChainDesc, &AppComponents.pSwapChain, &AppComponents.pD3D10Device);
	AppComponents.pSwapChain->GetBuffer(NULL, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&AppComponents.pBackBuffer));
	AppComponents.pD3D10Device->CreateRenderTargetView(AppComponents.pBackBuffer, NULL, &AppComponents.pRenderTargetView);

	GameInput2::GetInstance().Initialize(HWnd);

	onResize();

	return true;
}

void CGameApp::Release()
{
	UnregisterClass(WndClassName, HInstance);
	//CloseWindow(HWnd);
	DestroyWindow(HWnd);
	Utils::Release(&AppComponents.pRasterizeState);
	Utils::Release(&AppComponents.pDepthStencilView);
	Utils::Release(&AppComponents.pDepthDisableStencilState);
	Utils::Release(&AppComponents.pDepthStencilState);
	Utils::Release(&AppComponents.pBackBuffer);
	Utils::Release(&AppComponents.pRenderTargetView);
	Utils::Release(&AppComponents.pSwapChain);
	Utils::Release(&AppComponents.pD3D10RenderView);
	Utils::Release(&AppComponents.pD3D10Device);

	for (auto& adapter : AdaptersList)
	{
		Utils::Release(&adapter);
	}

	for (auto& output : AdaptersOutputsList)
	{
		Utils::Release(&output);
	}

	CGameLevelLoader::GetInstance().UnloadLevel();
	CGameAssetLoader::GetInstance().Release();
}

void CGameApp::CalculateFrameStatus()
{
	static int frameCnt = 0;	//프레임 카운트
	static float timeElapsed = 0.0f;	//흐른 시간

	frameCnt++; 

	//1초가 지났을 때 프레임 정보 업데이트
	if (GameTimer.GetTotalTime() - timeElapsed >= 1.0f) 
	{
		float fps = (float)frameCnt;
		float mspf = 1000.0f / fps;
	
		std::wostringstream outs;
		outs.precision(6);
		outs << FrameTitle << L"  Adapter: "<< AdapterInfoList[1].Description << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(HWnd, outs.str().c_str());

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

	CGameLevelLoader::GetInstance().LoadLevel(new LobbyLevel());

	GameTimer.Reset();
	GameTimer.Start();

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
		if (GameInput2::GetInstance().IsPressed(VK_ESCAPE))
		{
			return;
		}
		AppComponents.pD3D10Device->ClearRenderTargetView(AppComponents.pRenderTargetView, GameColors::BLUE);

		AppComponents.pD3D10Device->ClearDepthStencilView(AppComponents.pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

		Update();
		if (!Render())
		{
			//Error...
		}

		POINT pos = GameInput2::GetInstance().GetMousePosition();

		AppComponents.pSwapChain->Present(0, 0);

	}
}


void CGameApp::Update()
{
	GameTimer.Tick();
	//GameInput::GetInstance().Frame();
	CGameLevelLoader::GetInstance().UpdateLevel(GameTimer.GetDeltaTime());
	CalculateFrameStatus();
}


bool CGameApp::Render()
{
	TurnOffZBuffer();

	CGameLevelLoader::GetInstance().RenderLevel(AppComponents.pD3D10Device, Windowsize.width, Windowsize.height);

	TurnOnZBuffer();

	return true;
}


void CGameApp::onResize()
{
	GAME_ASSERT(AppComponents.pD3D10Device, "D3D11Device is nullptr");
	GAME_ASSERT(AppComponents.pSwapChain, "SwapChain is nullptr");

	//재할당 전 해제
	Utils::Release(&AppComponents.pRenderTargetView);
	Utils::Release(&AppComponents.pDepthStencilView);
	Utils::Release(&AppComponents.pBackBuffer);

	RECT rect; GetWindowRect(HWnd, &rect);
	Windowsize.width  = rect.right - rect.left;
	Windowsize.height = rect.bottom - rect.top;

	//스왑체인 버퍼 사이즈 재설정
	AppComponents.pSwapChain->ResizeBuffers(1, Windowsize.width, Windowsize.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D10Texture2D* dxgiTextureBuffer = nullptr;
	AppComponents.pSwapChain->GetBuffer(NULL, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&dxgiTextureBuffer));
	AppComponents.pD3D10Device->CreateRenderTargetView(dxgiTextureBuffer, 0, &AppComponents.pRenderTargetView);
	Utils::Release(&dxgiTextureBuffer);

	//깊이 버퍼 재설정
	D3D10_TEXTURE2D_DESC depthBufferDesc;
		depthBufferDesc.Width     = Windowsize.width;
		depthBufferDesc.Height    = Windowsize.height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if (MultiSampleQualityLevel) 
		{
			depthBufferDesc.SampleDesc.Count   = 4;
			depthBufferDesc.SampleDesc.Quality = MultiSampleQualityLevel - 1;
		}
		else 
		{
			depthBufferDesc.SampleDesc.Count   = 1;
			depthBufferDesc.SampleDesc.Quality = 0;
		}
		depthBufferDesc.Usage          = D3D10_USAGE_DEFAULT;
		depthBufferDesc.BindFlags      = D3D10_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = NULL;
		depthBufferDesc.MiscFlags      = NULL;
	AppComponents.pD3D10Device->CreateTexture2D(&depthBufferDesc, NULL, &AppComponents.pBackBuffer);
	
	//
	D3D10_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D10_BLEND_DESC));
		blendStateDesc.AlphaToCoverageEnable = FALSE;
		blendStateDesc.BlendEnable[0] = TRUE;
		blendStateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
		blendStateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
		blendStateDesc.BlendOp = D3D10_BLEND_OP_ADD;
		blendStateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
		blendStateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
		blendStateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		blendStateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	AppComponents.pD3D10Device->CreateBlendState(&blendStateDesc, &AppComponents.pBlendState);
	AppComponents.pD3D10Device->OMSetBlendState(AppComponents.pBlendState, NULL, 0xFFFFFF);


	//깊이 스텐실 버퍼 생성 -> Z버퍼 켬
	D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D10_DEPTH_STENCIL_DESC));
		depthStencilDesc.DepthEnable    = true;
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc      = D3D10_COMPARISON_LESS;
		///
		depthStencilDesc.StencilEnable    = true;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.StencilReadMask  = 0xFF;
		///
		depthStencilDesc.FrontFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;
		///
		depthStencilDesc.BackFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;
	AppComponents.pD3D10Device->CreateDepthStencilState(&depthStencilDesc, &AppComponents.pDepthStencilState);

	//깊이 스텐실 버퍼 생성 -> Z버퍼 끔
	D3D10_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
		depthDisabledStencilDesc.DepthEnable    = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc      = D3D10_COMPARISON_LESS;
		///
		depthDisabledStencilDesc.StencilEnable    = true;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.StencilReadMask  = 0xFF;
		///
		depthDisabledStencilDesc.FrontFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;
		///
		depthDisabledStencilDesc.BackFace.StencilFailOp      = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp      = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc        = D3D10_COMPARISON_ALWAYS;
	AppComponents.pD3D10Device->CreateDepthStencilState(&depthDisabledStencilDesc, &AppComponents.pDepthDisableStencilState);
	//기본 Z버퍼 사용 버퍼로 설정
	AppComponents.pD3D10Device->OMSetDepthStencilState(AppComponents.pDepthStencilState, 1);

	//깊이 스텐실 뷰 생성 및 설정
	D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D10_DEPTH_STENCIL_VIEW_DESC));
		depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension      = D3D10_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	AppComponents.pD3D10Device->CreateDepthStencilView(AppComponents.pBackBuffer, &depthStencilViewDesc, &AppComponents.pDepthStencilView);
	AppComponents.pD3D10Device->OMSetRenderTargets(1, &AppComponents.pRenderTargetView, AppComponents.pDepthStencilView);

	//레스터라이저 설정
	D3D10_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode              = D3D10_CULL_BACK;
		rasterDesc.DepthBias             = 0;
		rasterDesc.DepthBiasClamp        = 0.0f;
		rasterDesc.DepthClipEnable       = true;
		rasterDesc.FillMode              = D3D10_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable     = false;
		rasterDesc.ScissorEnable         = false;
		rasterDesc.SlopeScaledDepthBias  = 0.0f;
	AppComponents.pD3D10Device->CreateRasterizerState(&rasterDesc, &AppComponents.pRasterizeState);
	AppComponents.pD3D10Device->RSSetState(AppComponents.pRasterizeState);

	//뷰포트 설정
	D3D10_VIEWPORT viewportSettings;
		viewportSettings.TopLeftX = 0.0f;
		viewportSettings.TopLeftY = 0.0f;
		viewportSettings.Width    = static_cast<float>(Windowsize.width);
		viewportSettings.Height   = static_cast<float>(Windowsize.height);
		viewportSettings.MinDepth = 0.0f;
		viewportSettings.MaxDepth = 1.0f;
	AppComponents.pD3D10Device->RSSetViewports(1, &viewportSettings);

	float fieldOfView  = static_cast<float>(D3DX_PI) / 4.0f;
	float screenAspect = static_cast<float>(Windowsize.width) / static_cast<float>(Windowsize.height);

	D3DXMatrixPerspectiveFovLH(&GameMatrices.projectionMatrix, fieldOfView, screenAspect, ScreenNear, ScreenDepth);
	D3DXMatrixIdentity(&GameMatrices.worldMatrix);
	D3DXMatrixOrthoLH(&GameMatrices.orthMatrix, static_cast<float>(Windowsize.width), static_cast<float>(Windowsize.height), ScreenNear, ScreenDepth);

	if (!TextureShader::GetInstance().Initialize(AppComponents.pD3D10Device, HWnd))
	{
		MessageBox(HWnd, L"Error!!", L"Cannot Initialize Texture Shaders!", MB_OK);
		return;
	}

	CGameAssetLoader::GetInstance().Initialize(AppComponents.pD3D10Device, &Windowsize.width, &Windowsize.height);

	onShowWindow();
}

void CGameApp::onShowWindow()
{
	ShowWindow(HWnd, SW_SHOW);
	ShowCursor(false);
	SetForegroundWindow(HWnd);
	SetFocus(HWnd);
	UpdateWindow(HWnd);
}

void CGameApp::TurnOnZBuffer()
{
	AppComponents.pD3D10Device->OMSetDepthStencilState(AppComponents.pDepthStencilState, 1);
}

void CGameApp::TurnOffZBuffer()
{
	AppComponents.pD3D10Device->OMSetDepthStencilState(AppComponents.pDepthDisableStencilState, 1);
}

CGameApp& CGameApp::GetInstance()
{
	static CGameApp Instance;

	return Instance;
}

D3DXMATRIX& CGameApp::GetProjectionMatrix()
{
	return GameMatrices.projectionMatrix;
}

D3DXMATRIX& CGameApp::GetWorldMatrix()
{
	return GameMatrices.worldMatrix;
}

D3DXMATRIX& CGameApp::GetorthogonalMatrix()
{
	return GameMatrices.orthMatrix;
}

WINDOWSIZE CGameApp::GetWindowSize() const
{
	return Windowsize;
}

HWND CGameApp::GetHWND() const
{
	return HWnd;
}

ID3D10Device* CGameApp::GetDevice() const
{
	return AppComponents.pD3D10Device;
}

const AppInfo* CGameApp::GetAppInfo() const
{
	return &AppComponents;
}