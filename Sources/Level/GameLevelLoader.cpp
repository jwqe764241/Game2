#include <Sources/Level/GameLevelLoader.h>


CGameLevelLoader::CGameLevelLoader() : m_CurrentLevel(nullptr)
{
	/*m_PreLoadedLevel.insert(std::pair<int, ILevel*>(LevelID::LEVEL_MAIN, new LobbyLevel()));
	m_PreLoadedLevel.insert(std::pair<int, ILevel*>(LevelID::LEVEL_DUMMY, new DummyLevel()));
	m_PreLoadedLevel.insert(std::pair<int, ILevel*>(LevelID::LEVEL_TEST1, new TestLevel1()));

	for (auto& level : m_PreLoadedLevel)
	{
		level.second->Load();
	}*/
}

CGameLevelLoader::~CGameLevelLoader()
{
	/*for (auto& level : m_PreLoadedLevel)
	{
		delete level.second;
		level.second = nullptr;
	}*/

	UnloadLevel();
}

CGameLevelLoader& CGameLevelLoader::GetInstance()
{
	static CGameLevelLoader instance;

	return instance;
}

bool CGameLevelLoader::LoadLevel(ILevel* level)
{
	if (level)
	{
		m_CurrentLevel = level;
		if (!m_CurrentLevel->Load())
		{
			return false;
		}

		return true;
	}
	else
	{
		//유효하지 않은 레벨
		return false;
	}
}

//void CGameLevelLoader::LoadLevel(LevelID id)
//{
//	m_CurrentLevel = m_PreLoadedLevel.find(id)->second;
//}

void CGameLevelLoader::UnloadLevel()
{
	if (m_CurrentLevel) 
	{
		delete m_CurrentLevel;
		m_CurrentLevel = nullptr;
	}
}

bool CGameLevelLoader::ChangeLevel(ILevel* level)
{
	if (level)
	{
		UnloadLevel();
		if (!LoadLevel(level))
		{
			return false;
		}

		return true;
	}
	else
	{
		//유효하지 않은 레벨
		return false;
	}
}

//void CGameLevelLoader::ChangeLevel(LevelID id)
//{
//	UnloadLevel();
//	LoadLevel(id);
//}

void CGameLevelLoader::UpdateLevel(float dt)
{
	if (m_CurrentLevel)
	{
		m_CurrentLevel->Update(dt);
	}
}

bool CGameLevelLoader::RenderLevel(ID3D10Device* device, int screenWidth, int screenHeight)
{
	if (m_CurrentLevel)
	{
		if (!m_CurrentLevel->Render(device, screenWidth, screenHeight))
		{
			return false;
		}

		return true;
	}
	else
	{
		//유효하지 않은 레벨
		return false;
	}
}