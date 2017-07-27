#include <Sources/GameAssetLoader.h>

CGameAssetLoader::CGameAssetLoader()
{

}

CGameAssetLoader::~CGameAssetLoader()
{

}

void CGameAssetLoader::Release()
{
	ClearMap();
}

void CGameAssetLoader::LoadAsset(IRenderable* target, int id)
{
	m_AssetMap.insert(std::pair<int, TargetInterface*>(id, target));
}

TargetInterface* CGameAssetLoader::GetAsset(int id)
{
	return m_AssetMap.find(id)->second;
}

void CGameAssetLoader::ClearMap()
{
	for (auto itor = m_AssetMap.begin(); itor != m_AssetMap.end(); itor++)
	{
		delete itor->second;
	}

	m_AssetMap.clear();
}
