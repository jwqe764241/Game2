#include <Sources/Level/GameLevelLoader.h>


CGameLevelLoader::CGameLevelLoader() : m_CurrentLevel(nullptr)
{

}

//CGameLevelLoader::CGameLevelLoader(ILevel* pLevel)
//{
//	if (pLevel) 
//	{
//		m_CurrentLevel = pLevel;
//	}
//}

CGameLevelLoader::~CGameLevelLoader()
{
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

void CGameLevelLoader::UnloadLevel()
{
	if (m_CurrentLevel) 
	{
		m_CurrentLevel->Unload();
		delete m_CurrentLevel;
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

void CGameLevelLoader::UpdateLevel(float dt)
{
	if (m_CurrentLevel)
	{
		m_CurrentLevel->Update(dt);
	}
}

bool CGameLevelLoader::RenderLevel(ID3D11DeviceContext* deviceContext)
{
	if (m_CurrentLevel)
	{
		if (!m_CurrentLevel->Render(deviceContext))
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