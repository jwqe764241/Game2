#ifndef __GAMESETTINGS_H
#define __GAMESETTINGS_H

#include <D3DX11.h>
#include <D3D11.h>

struct CGameSettings {
	D3D_DRIVER_TYPE		 DriverType = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL	 FeatureLevel;
	UINT				 SDKVersion = D3D11_SDK_VERSION;
	UINT				 MultiSampleQualityLevel = NULL;
	D3D11_VIEWPORT		 ViewportSettings;
	DXGI_SWAP_CHAIN_DESC SwapChainDescribe;
};

#endif