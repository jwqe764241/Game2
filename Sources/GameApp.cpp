#include <Sources/GameApp.h>
#include <Sources/Utils/Release.h>


HRESULT hr;

CGameApp::CGameApp(HINSTANCE hInstance, wchar_t * frameTitle, wchar_t * wndClassName, int nCmdShow, int width, int height)
	: m_pD3D11Device(nullptr), m_pD3D11DeviceContext(nullptr), m_pGameWindow(nullptr), m_pDxgiSwapChain(nullptr),
	m_DriverType(D3D_DRIVER_TYPE_HARDWARE),m_SDKVersion(D3D11_SDK_VERSION), m_MultiSampleQualityLevel(NULL)
{

	m_pGameWindow = new CGameWindow(hInstance, frameTitle, wndClassName, nCmdShow, width, height);



	UINT createFlags = 0;
#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDevice(
		0,
		m_DriverType,
		0,
		createFlags,
		NULL, NULL,
		m_SDKVersion,
		&m_pD3D11Device,
		&m_FeatureLevel,
		&m_pD3D11DeviceContext
	);

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
		m_pD3D11Device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MultiSampleQualityLevel
		);
		GAME_ASSERT(m_MultiSampleQualityLevel > 0, "Unexpected ERROR");
	}


	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width						= m_pGameWindow->GetWidth();
	swapChainDesc.BufferDesc.Height						= m_pGameWindow->GetHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	
	if (m_MultiSampleQualityLevel) {
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_MultiSampleQualityLevel - 1;
	}
	else {
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}


	IDXGIDevice * dxgiDevice = nullptr;
	m_pD3D11Device->QueryInterface(
		__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)
	);
	IDXGIAdapter * dxgiAdapter = nullptr;
	dxgiDevice->GetParent(
		__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter)
	);
	IDXGIFactory * dxgiFactory = nullptr;
	dxgiAdapter->GetParent(
		__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)
	);
	dxgiFactory->QueryInterface(
		__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pDxgiSwapChain)
	);

	dxgiFactory->CreateSwapChain(
		m_pD3D11Device, &swapChainDesc, &m_pDxgiSwapChain
	);

	ReleaseCOM(&dxgiDevice);
	ReleaseCOM(&dxgiAdapter);
	ReleaseCOM(&dxgiFactory);
}


CGameApp::~CGameApp()
{
	delete m_pGameWindow;
	ReleaseCOM(&m_pDxgiSwapChain);
}


void CGameApp::Launch() {
	
	m_pGameWindow->StartWindow();

}