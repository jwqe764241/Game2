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

		#if defined (_DEBUG)
			GAME_ASSERT(0 != 0, "Failed - D3D11CreateDevice");
		#else
			if (MessageBox(NULL, L"Failed - D3D11CreateDevice", L"ERROR!!", MB_OK) == IDOK) {
				if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
					PostQuitMessage(0);
				}
			}
		#endif
	}

	if (m_FeatureLevel != D3D_FEATURE_LEVEL_11_0) {
		#if defined (_DEBUG)
			GAME_ASSERT(0 != 0, "Failed - Not Support Directx 11");
		#else
			if (MessageBox(NULL, L"Failed - Not Support Directx 11", L"ERROR!!", MB_OK) == IDOK) {
				if (MessageBox(NULL, L"Click OK Button to Shut Down this program", L"Shut Down", MB_OK) == IDOK) {
					PostQuitMessage(0);
				}
			}
		#endif
	}

	

}


CGameApp::~CGameApp()
{
}
