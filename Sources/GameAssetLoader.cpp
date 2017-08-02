#include <Sources/GameAssetLoader.h>

CGameAssetLoader::CGameAssetLoader()
{

}

CGameAssetLoader::~CGameAssetLoader()
{
	Release();
}

CGameAssetLoader& CGameAssetLoader::GetInstance()
{
	static CGameAssetLoader instance;

	return instance;
}

void CGameAssetLoader::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int* screenWidth, int* screenHeight)
{
	m_AllocList.reserve(50);
	m_DeviceRef = device;
	m_DeviceContextRef = deviceContext;
	m_ScreenWidthRef = screenWidth;
	m_ScreenHeightRef = screenHeight;
}

void CGameAssetLoader::Release()
{
	for (auto allocTarget : m_AllocList)
	{
		Utils::Release(&allocTarget);
	}
}

TargetInterface* CGameAssetLoader::LoadAsset(int id, int bitmapWidth, int bitmapHeight)
{

	TargetInterface* asset = nullptr;

	switch (id)
	{
	case ID_ASSET_TESTASSET:
		asset = new TestAsset();
		break;
	case ID_ASSET_SPRITEASSET:
		asset = new SpriteAsset();
		break;
	case ID_ASSET_PLAYER:
		asset = new Player();
		break;
	case ID_ASSET_ENEMY1:
		asset = new Enemy1();
	}

	asset->Load(m_DeviceRef, bitmapWidth, bitmapHeight);
	m_AllocList.push_back(asset);

	return asset;
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
