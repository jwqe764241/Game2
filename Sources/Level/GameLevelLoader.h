#ifndef __GAMELEVELLOADER_H
#define __GAMELEVELLOADER_H

#include <Sources/Objects/ILevel.h>

class CGameLevelLoader {
private:
	ILevel* m_CurrentLevel;

public:
	CGameLevelLoader();
	//CGameLevelLoader(ILevel* pLevel);
	CGameLevelLoader(CGameLevelLoader&) = delete;
	~CGameLevelLoader();

	static CGameLevelLoader& GetInstance();

	bool LoadLevel(ILevel* level);
	void UnloadLevel();
	bool ChangeLevel(ILevel* level);
	void UpdateLevel(float dt);
	bool RenderLevel();
};

#endif