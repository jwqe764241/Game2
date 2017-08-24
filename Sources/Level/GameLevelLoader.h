#pragma once

#include <map>
#include <D3D11.h>
#include <Sources/Interface/ILevel.h>

#include <Sources/Level/DummyLevel.h>
#include <Sources/Level/LobbyLevel.h>
#include <Sources/Level/TestLevel1.h>

class CGameLevelLoader {
private:
	ILevel* m_CurrentLevel;
	//std::map<int, ILevel*> m_PreLoadedLevel;
public:
	enum LevelID {
		LEVEL_MAIN,
		LEVEL_DUMMY,
		LEVEL_TEST1
	};

public:
	CGameLevelLoader();
	CGameLevelLoader(CGameLevelLoader&) = delete;
	~CGameLevelLoader();

	static CGameLevelLoader& GetInstance();

	bool LoadLevel(ILevel* level);
	//void LoadLevel(LevelID id);
	void UnloadLevel();
	bool ChangeLevel(ILevel* level);
	//void ChangeLevel(LevelID id);
	void UpdateLevel(float dt);
	bool RenderLevel(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight);
};