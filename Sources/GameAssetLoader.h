#ifndef __GAMEASSETLOADER
#define __GAMEASSETLOADER

#include <map>

#include <Sources/Objects/IRenderable.h>

using TargetInterface = IRenderable;

class CGameAssetLoader {
	//변경될 수 있으므로 

public:
	CGameAssetLoader();
	~CGameAssetLoader();
	
	void Release();

	//에셋 불러온 후 Map에 저장
	void LoadAsset(IRenderable* target, int id);
	TargetInterface* GetAsset(int id);

private:
	void ClearMap();

private:
	//ICharacter가 맞는가...
	std::map<int, TargetInterface*> m_AssetMap;
	
};

#endif