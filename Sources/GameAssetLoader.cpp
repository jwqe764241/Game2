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

void CGameAssetLoader::Initialize(ID3D10Device* device, int* screenWidth, int* screenHeight)
{
	AllocationList.reserve(50);
	DeviceRef = device;
	ScreenWidthRef = screenWidth;
	ScreenHeightRef = screenHeight;
}

void CGameAssetLoader::Release()
{
	for (auto allocTarget : AllocationList)
	{
		Utils::Release(&allocTarget);
	}
}

IRenderable* CGameAssetLoader::LoadAsset(int id, int bitmapWidth, int bitmapHeight)
{

	IRenderable* asset = nullptr;

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

	asset->Load(DeviceRef, bitmapWidth, bitmapHeight);
	AllocationList.push_back(asset);

	return asset;
}