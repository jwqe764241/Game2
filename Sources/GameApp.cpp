#include <Sources/GameApp.h>

#define ENABLE_PRINT_ADAPTER_NAME

//������ ���ν���
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
		std::cout << "width : " << LOWORD(lParam) << " height : " << HIWORD(lParam) << std::endl;
		onResize();
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

CGameApp::CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height, float screenDepth, float screenNear)
	: m_AppInfo({nullptr}), m_vAdapters(), m_screenDepth(screenDepth), m_screenNear(screenNear),
	m_hInstance(hInstance), m_pstrFrameTitle(frameTitle), m_pstrWndClassName(wndClassName), m_iCmdShow(nCmdShow),
	m_TextureShader(nullptr), m_Bitmap(nullptr)
{
	m_vAdapters.reserve(10);

	//������ Ŭ���� ����
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
	//������ �ӽ� ���� ������ ������ ���� --> ���?���ν����� �ѱ��?����

	//0�� ���?�⺻ ũ���?����
	if (width == 0 || height == 0) {
		m_sizeWindow = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	}
	else {
		m_sizeWindow.width = width;
		m_sizeWindow.height = height;
	}

	//������ ����
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW ,
		m_pstrWndClassName,
		m_pstrFrameTitle,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_sizeWindow.width,
		m_sizeWindow.height,
		NULL,
		NULL,
		m_hInstance,
		this);

	SetWindowLongPtr(m_hWnd, 0, reinterpret_cast<LONG_PTR>(this));

	m_Input = new GameInput;

	//-- HRESULT üŷ ���� --//
	//���丮 ����
	IDXGIFactory* l_pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&l_pFactory));
	
	//�����?����
	IDXGIAdapter* l_pAdapter = nullptr;
	IDXGIOutput * l_pOutput = nullptr;

	//�����?���Ϳ� Ǫ��
	for (int i = 0; l_pFactory->EnumAdapters(i, &l_pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		m_vAdapters.push_back(l_pAdapter);
	}
	
	std::cout << "-----------------------�νĵ� �����?����-----------------------" << std::endl;
	for (int i = 0; i < m_vAdapters.size(); i++) {
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


	//����Ʈ ���� ���?����
	unsigned int l_uiModes = 0;
	m_vAdaptersOutputs[0]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, NULL);
	//�������?���� ��, �Ӽ� ���?
	DXGI_MODE_DESC * l_pModeList = new DXGI_MODE_DESC[l_uiModes];
	m_vAdaptersOutputs[0]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &l_uiModes, l_pModeList);

	std::cout << "-----------------------�νĵ� �����?����-----------------------" << std::endl;
	for (int i = 0; i < l_uiModes; i++) {
		std::cout << "\n" << i << "�� �����?�ػ� (width X height)" << l_pModeList[i].Width << "X" << l_pModeList[i].Height << std::endl;
		std::cout << "���� : " << l_pModeList[i].Format << std::endl;
		std::cout << "������ : " << l_pModeList[i].RefreshRate.Denominator << "/" << l_pModeList[i].RefreshRate.Numerator << std::endl;
	}
	std::cout << "----------------------------------------------------------------" << std::endl;


	//���ΰ�ħ ���� ���� �и� ���?
	//60hrz -> msi ��Ʈ��
	int l_iNumerator, l_iDenominator;
	for (int i = 0; i < l_uiModes; ++i) {
		if ((l_pModeList[i].Width == m_sizeWindow.width) &&
			(l_pModeList[i].Height == m_sizeWindow.height)) {
			l_iNumerator	= l_pModeList[i].RefreshRate.Numerator;
			l_iDenominator	= l_pModeList[i].RefreshRate.Denominator;
		}
	}

	
	//�����?������ ���Ϳ� �߰�
	for (int i = 0; i < m_vAdapters.size(); ++i){
		DXGI_ADAPTER_DESC l_pAdapterDesc;
		m_vAdapters[i]->GetDesc(&l_pAdapterDesc);

		AdapterInfo adapterInfo;
		adapterInfo.Description = l_pAdapterDesc.Description;
		adapterInfo.ID          = l_pAdapterDesc.AdapterLuid;

		m_vAdapterInfoList.push_back(adapterInfo);
	}
	

	//����
	delete[] l_pModeList;
	Utils::Release(&l_pFactory);

	//����ü�� ����
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

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, 
		&swapChainDesc, &m_AppInfo.pSwapChain, &m_AppInfo.pD3D11Device, NULL, &m_AppInfo.pD3D11DeviceContext);
	m_AppInfo.pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_AppInfo.pBackBuffer));
	m_AppInfo.pD3D11Device->CreateRenderTargetView(m_AppInfo.pBackBuffer, NULL, &m_AppInfo.pRenderTargetView);

	onResize();

	/*
	//�����?���ؼ� ����Ʈ�� ����
	IDXGIFactory * pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	UINT i = 0;
	IDXGIAdapter * pAdapter = nullptr;
	while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_vAdapters.push_back(pAdapter);
		++i;
	}
	Utils::Release(&pFactory);

	//�����?���� �÷���
	UINT createFlags = 0;
#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//DirectX ����̽�?����
	//pAdapter != null �� ����̹�?Ÿ���� -> D3D_DRIVER_TYPE_UNKNOWN
	D3D11CreateDevice(m_vAdapters.at(1),D3D_DRIVER_TYPE_UNKNOWN,0,createFlags,NULL,NULL,m_SDKVersion,&m_pD3D11Device,&m_FeatureLevel,&m_pD3D11DeviceContext);

	//���� üũ -> ����̽�?���� ����
	if (FAILED(hr)) {
		GAME_ASSERT(0 != 0, "Failed - D3D11CreateDevice");

		if (MessageBox(NULL, L"Failed - D3D11CreateDevice", L"ERROR!!", MB_OK) == IDOK) {
			if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
				PostQuitMessage(0);
			}
		}
	}
	
	//���� üũ -> D11 ���� ����
	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0) {
		GAME_ASSERT(0 != 0, "Failed - Not Support Directx 11");

		if (MessageBox(NULL, L"Failed - Not Support Directx 11", L"Error!!", MB_OK) == IDOK) {
			if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
				PostQuitMessage(0);
			}
		}
	}

	//��Ƽ ���� ���� üũ
	if (m_pD3D11Device != nullptr) {
		CHECK_HR(m_pD3D11Device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MultiSampleQualityLevel
		));
		GAME_ASSERT(m_MultiSampleQualityLevel > 0, "Unexpected ERROR");
	}


	//���� ü�� ����
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
	

	//���� ü�� ����
	IDXGIDevice * dxgiDevice = nullptr;
	m_pD3D11Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	IDXGIAdapter * dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));
	IDXGIFactory * dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
	//dxgiFactory->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
	dxgiFactory->CreateSwapChain(m_pD3D11Device, &swapChainDesc, &m_pSwapChain);
	//dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		Utils::Release(&dxgiDevice);
		Utils::Release(&dxgiAdapter);
		Utils::Release(&dxgiFactory);;

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
	Utils::Release(&m_TextureShader);
	Utils::Release(&m_Bitmap);
	Utils::Release(&m_AppInfo.pRasterizeState);
	Utils::Release(&m_AppInfo.pDepthStencilView);
	Utils::Release(&m_AppInfo.pDepthDisableStencilState);
	Utils::Release(&m_AppInfo.pDepthStencilState);
	Utils::Release(&m_AppInfo.pBackBuffer);
	Utils::Release(&m_AppInfo.pRenderTargetView);
	Utils::Release(&m_AppInfo.pSwapChain);
	Utils::Release(&m_AppInfo.pD3D11DeviceContext);
	Utils::Release(&m_AppInfo.pD3D11Device);

	for (IDXGIAdapter* adapter : m_vAdapters) {
		Utils::Release(&adapter);
	}

	for (IDXGIOutput* output : m_vAdaptersOutputs) {
		Utils::Release(&output);
	}

	delete m_Camera;
	m_Camera = nullptr;
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
		outs << m_pstrFrameTitle << L"  Adapter: "<< m_vAdapterInfoList[1].Description << L"    "
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

	m_Input->Initialize(m_hInstance, m_hWnd, m_sizeWindow.width, m_sizeWindow.height);

	m_GameTimer.Reset();
	m_GameTimer.Start();

	MSG msg = { 0 };
	bool bIsRunning = true;

	//�����?��忡��?�����?�̸� ���?���� �� �������?
#if defined(_DEBUG) && defined(ENABLE_PRINT_ADAPTER_NAME)
	for (IDXGIAdapter * adapter : m_vAdapters) {
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);
		wprintf(L"%s \n", adapterDesc.Description);
	}
#endif

	//�޽��� ����
	while (msg.message != WM_QUIT) {

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_AppInfo.pD3D11DeviceContext->ClearRenderTargetView(m_AppInfo.pRenderTargetView,
			GameColors::BLACK
		);

		m_AppInfo.pD3D11DeviceContext->ClearDepthStencilView(
			m_AppInfo.pDepthStencilView,
			D3D11_CLEAR_DEPTH, 1.0f, 0
		);

		Update();
		Render();

		m_AppInfo.pSwapChain->Present(0, 0);
	}
}


void CGameApp::Update()
{
	m_GameTimer.Tick();
	m_Input->Frame();
	CalculateFrameStatus();
}


void CGameApp::Render()
{
	TurnOffZBuffer();

	if (!m_Bitmap->Render(m_AppInfo.pD3D11DeviceContext, 100, 100)) 
	{
		MessageBox(m_hWnd, L"dsfsd", L"Bitmapsad", MB_OK);
	}

	if (!m_TextureShader->Render(m_AppInfo.pD3D11DeviceContext, m_Bitmap->GetIndexCount(),
		m_Matrix.worldMatrix, m_Camera->GetViewMatrix(), m_Matrix.orthMatrix, m_Bitmap->GetTexture()))
	{
		MessageBox(m_hWnd, L"dsfsd", L"TextureShaderssd", MB_OK);
	}

	TurnOnZBuffer();
}


void CGameApp::onResize()
{
	GAME_ASSERT(m_AppInfo.pD3D11DeviceContext, "D3D11Device is nullptr");
	GAME_ASSERT(m_AppInfo.pD3D11Device, "D3D11Device is nullptr");
	GAME_ASSERT(m_AppInfo.pSwapChain, "SwapChain is nullptr");

	//������ ����ϴ���?����
	Utils::Release(&m_AppInfo.pRenderTargetView);
	Utils::Release(&m_AppInfo.pDepthStencilView);
	Utils::Release(&m_AppInfo.pBackBuffer);

	RECT rect; GetWindowRect(m_hWnd, &rect);
	m_sizeWindow.width  = rect.right - rect.left;
	m_sizeWindow.height = rect.bottom - rect.top;

	m_Input->Shutdown();
	m_Input->Initialize(m_hInstance, m_hWnd, m_sizeWindow.width, m_sizeWindow.height);

	//���� �������� �� ����Ÿ�ٺ� �����?
	m_AppInfo.pSwapChain->ResizeBuffers(1, m_sizeWindow.width, m_sizeWindow.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* dxgiTextureBuffer = nullptr;
	m_AppInfo.pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&dxgiTextureBuffer));
	m_AppInfo.pD3D11Device->CreateRenderTargetView(dxgiTextureBuffer, 0, &m_AppInfo.pRenderTargetView);
	Utils::Release(&dxgiTextureBuffer);

	//���ٽ� ���� ����
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
	m_AppInfo.pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_AppInfo.pBackBuffer);
	
	//���ٽ� ����
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
	//�⺻ ZBuffer ���� �� ���۷� ����
	m_AppInfo.pD3D11DeviceContext->OMSetDepthStencilState(m_AppInfo.pDepthStencilState, 1);

	//���� ���ٽ� �� ����
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
	m_AppInfo.pD3D11Device->CreateDepthStencilView(m_AppInfo.pBackBuffer, &depthStencilViewDesc, &m_AppInfo.pDepthStencilView);
	m_AppInfo.pD3D11DeviceContext->OMSetRenderTargets(1, &m_AppInfo.pRenderTargetView, m_AppInfo.pDepthStencilView);

	//�����Ͷ����� ����
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

	//����Ʈ ����
	D3D11_VIEWPORT viewportSettings;
		viewportSettings.TopLeftX = 0.0f;
		viewportSettings.TopLeftY = 0.0f;
		viewportSettings.Width    = static_cast<float>(m_sizeWindow.width);
		viewportSettings.Height   = static_cast<float>(m_sizeWindow.height);
		viewportSettings.MinDepth = 0.0f;
		viewportSettings.MaxDepth = 1.0f;
	m_AppInfo.pD3D11DeviceContext->RSSetViewports(1, &viewportSettings);

	float fieldOfView  = static_cast<float>(D3DX_PI) / 4.0f;
	float screenAspect = static_cast<float>(m_sizeWindow.width) / static_cast<float>(m_sizeWindow.height);

	D3DXMatrixPerspectiveFovLH(&m_Matrix.projectionMatrix, fieldOfView, screenAspect, m_screenNear, m_screenDepth);
	D3DXMatrixIdentity(&m_Matrix.worldMatrix);
	D3DXMatrixOrthoLH(&m_Matrix.orthMatrix, static_cast<float>(m_sizeWindow.width), static_cast<float>(m_sizeWindow.height), m_screenNear, m_screenDepth);

	//���� �������� ���� ���� ����
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
		depthDisabledStencilDesc.DepthEnable    = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;
		///
		depthDisabledStencilDesc.StencilEnable    = true;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.StencilReadMask  = 0xFF;
		///
		depthDisabledStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
		///
		depthDisabledStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
	m_AppInfo.pD3D11Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_AppInfo.pDepthDisableStencilState);

	m_Camera = new CGameCamera();
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_TextureShader = new TextureShader();
	if (!m_TextureShader->Initialize(m_AppInfo.pD3D11Device, m_hWnd))
	{
		MessageBox(m_hWnd, L"Error!!", L"Cannot Initialize Texture Shaders!", MB_OK);
		return;
	}

	m_Bitmap = new GameBitmap();
	if (!m_Bitmap->Initialize(m_AppInfo.pD3D11Device, m_sizeWindow.width, m_sizeWindow.height, L"..\\Sources\\Geometries\\seafloor.dds", 256, 256))
	{
		MessageBox(m_hWnd, L"Error!!", L"Cannot Initialize Bitmap", MB_OK);
		return;
	}
}

void CGameApp::onShowWindow()
{
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	ShowCursor(true);
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

/*
void CGameApp::BuildShader()
{

}

void CGameApp::BuildVertexLayout()
{

}

#ifdef TEST_RENDER_BOX
void CGameApp::BuildBox() 
{
	//���� ������ ����
	Vertex::Vertex1 vertices[] = {
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::BLACK)	},
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::BLUE)	},
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::CYAN)	},
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::GREEN)	},
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::MAGENTA) },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::RED)		},
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::WHITE)	},
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), reinterpret_cast<const float*>(&CUSTOM_COLOR::YELLOW)	},
	};

	//���� ���� ���� ����
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Vertex1) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	//���� ���� ����ҽ�?����
	D3D11_SUBRESOURCE_DATA vinitdata;
	vinitdata.pSysMem = vertices;
	//���� ���� ����
	m_pD3D11Device->CreateBuffer(&vbd, &vinitdata, &m_pBoxVertexBuffer);

	
	//���� ������ ����
	UINT indices[] = {
		0, 1, 2,
		0, 2, 3,

		4, 6, 5,
		4, 7, 6,

		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
	};

	//���� ���� ���� ����
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	//���� ���� ����ҽ�?����
	D3D11_SUBRESOURCE_DATA iinitdata;
	iinitdata.pSysMem = indices;
	//���� ���� ����
	m_pD3D11Device->CreateBuffer(&ibd, &iinitdata, &m_pBoxIndexBuffer);
}
#endif
*/