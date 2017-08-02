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

//TODO: 확실해 지면 해당 것으로 수정하기
using TargetInterface = IRenderable;

class CGameAssetLoader {
	//변경될 수 있으므로 
public:
	CGameAssetLoader();
	~CGameAssetLoader();
	
	static CGameAssetLoader& GetInstance();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int* screenWidth, int* screenHeight);
	void Release();

	//에셋 불러온 후 Map에 저장
	TargetInterface* LoadAsset(int id, int bitmapWidth, int bitmapHeight);
	//TargetInterface* GetAsset(std::string id);

private:
	//void ClearMap();

private:
	//ICharacter가 맞는가...
	/*std::map<std::string, TargetInterface*> m_AssetMap;*/

	std::vector<TargetInterface *> m_AllocList;

	ID3D11Device * m_DeviceRef;
	ID3D11DeviceContext* m_DeviceContextRef;

	int* m_ScreenWidthRef;
	int* m_ScreenHeightRef;	
};