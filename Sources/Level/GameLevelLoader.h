#pragma once

#include <map>
#include <d3d10_1.h>
#include <Sources/Interface/ILevel.h>


class CGameLevelLoader 
{
public:
	enum LevelID {
		LEVEL_MAIN,
		LEVEL_DUMMY,
		LEVEL_TEST1
	};

private:
	ILevel* CurrentLevel;
	//std::map<int, ILevel*> m_PreLoadedLevel;

public:
	CGameLevelLoader();
	CGameLevelLoader(CGameLevelLoader&) = delete;
	virtual ~CGameLevelLoader();

	static CGameLevelLoader& GetInstance();

	bool LoadLevel(ILevel* level);
	//void LoadLevel(LevelID id);
	void UnloadLevel();
	bool ChangeLevel(ILevel* level);
	//void ChangeLevel(LevelID id);
	void UpdateLevel(float dt);
	bool RenderLevel(ID3D10Device* device, int screenWidth, int screenHeight);
};