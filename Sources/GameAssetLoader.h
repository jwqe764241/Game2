#pragma once

#include <vector>

#include <Sources/Assets/TestAsset.h>
#include <Sources/Assets/SpriteAsset.h>
#include <Sources/Assets/Player.h>
#include <Sources/Assets/Enemy1.h>
#include <Sources/Interface/ICharacter.h>
#include <Sources/GameDefs.h>

enum AssetID {
	//TetAsset은 말 그대로 테스트이기 때문에 테스트 위부터 아이디에 추가할 것
	ID_ASSET_TESTASSET = 50,
	ID_ASSET_SPRITEASSET = 60,
	ID_ASSET_PLAYER,
	ID_ASSET_ENEMY1
};

class CGameAssetLoader 
{
private:
	std::vector<IRenderable*> AllocationList;

	ID3D10Device* DeviceRef;

	int* ScreenWidthRef;
	int* ScreenHeightRef;

public:
	CGameAssetLoader();
	virtual ~CGameAssetLoader();
	
	static CGameAssetLoader& GetInstance();
	void Initialize(ID3D10Device* device, int* screenWidth, int* screenHeight);
	void Release();

	//에셋 불러온 후 Map에 저장
	IRenderable* LoadAsset(int id, int bitmapWidth, int bitmapHeight);
};