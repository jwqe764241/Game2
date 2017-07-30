#include <Sources/GameAssetLoader.h>

CGameAssetLoader::CGameAssetLoader()
{

}

CGameAssetLoader::~CGameAssetLoader()
{
	//Release();
}

CGameAssetLoader& CGameAssetLoader::GetInstance()
{
	static CGameAssetLoader instance;

	return instance;
}

void CGameAssetLoader::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int* screenWidth, int* screenHeight)
{
	m_DeviceRef = device;
	m_DeviceContextRef = deviceContext;
	m_ScreenWidthRef = screenWidth;
	m_ScreenHeightRef = screenHeight;
}

//void CGameAssetLoader::Release()
//{
//	//ClearMap();
//}

TargetInterface* CGameAssetLoader::LoadAsset(int id, int bitmapWidth, int bitmapHeight)
{

	TargetInterface* asset = nullptr;

	switch (id)
	{
	case ID_ASSET_TESTASSET:
		asset = new TestAsset();
		break;
	}

	asset->Load(m_DeviceRef, *m_ScreenWidthRef, *m_ScreenHeightRef, bitmapWidth, bitmapHeight);

	return asset;
	//m_AssetMap.insert(std::pair<std::string, TargetInterface*>(id, target));
}

//TargetInterface* CGameAssetLoader::GetAsset(std::string id)
//{
//	return m_AssetMap.find(id)->second;
//}
//
//void CGameAssetLoader::ClearMap()
//{
//	for (auto itor = m_AssetMap.begin(); itor != m_AssetMap.end(); itor++)
//	{
//		delete itor->second;
//	}
//
//	m_AssetMap.clear();
//}
