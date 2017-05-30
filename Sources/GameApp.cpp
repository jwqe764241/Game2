#include <Sources/GameApp.h>



HRESULT hr;

CGameApp::CGameApp(){

	UINT createFlags = 0;

#if defined(_DEBUG)
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDevice(
		0,
		m_DriveLevel,
		0,
		createFlags,
		NULL, NULL,
		D3D11_SDK_VERSION,
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
			DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MultiSampleQualityLevelVal
		);

		GAME_ASSERT(m_MultiSampleQualityLevelVal > 0, "Unexpected ERROR");
	}

}


CGameApp::~CGameApp()
{
}
