#include <Sources/Level/LobbyLevel.h>

LobbyLevel::LobbyLevel()
{

}

LobbyLevel::~LobbyLevel()
{
	Unload();
}

bool LobbyLevel::Load()
{
	for (int i = 0; i < 5; i++)
	{
		m_button[i].Load(CGameApp::GetInstance().GetDevice(), 300, 150);
		m_button[i].SetPosition(button_pos[i]);
	}

	m_Camera.SetPosition(0, 0, -10.0f);

	currSelection = 0;

	m_button[currSelection].Update(true);

	m_BackgroundBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/MainMenu.jpg", 1920, 1080);

	return true;
}

void LobbyLevel::Unload()
{
	for (auto button : m_button)
	{
		button.Release();
	}

	m_BackgroundBitmap.Release();
}

void LobbyLevel::Update(float dt)
{
	GameInput2& input = GameInput2::GetInstance();

	static bool upStatus = false;
	static bool downStatus = false;
	static bool spaceStatus = false;
	
	if (input.IsPressed(VK_UP))
	{
		upStatus = true;
		return;
	}
	else if (input.IsPressed(VK_DOWN))
	{
		downStatus = true;
		return;
	}
	else if (input.IsPressed(VK_SPACE))
	{
		spaceStatus = true;
		return;
	}

	if (upStatus && (currSelection > 0))
	{
		m_button[currSelection].Update(false);
		m_button[--currSelection].Update(true);
		upStatus = false;
	}
	else if (downStatus && (currSelection < 4))
	{
		m_button[currSelection].Update(false);
		m_button[++currSelection].Update(true);
		downStatus = false;
	}
	else if (spaceStatus)
	{
		switch (currSelection)
		{
		case ID_GAMESTART:
			{
				CGameLevelLoader::GetInstance().ChangeLevel(new TestLevel1());
				break;
			}
		case ID_GAMEINTRO:
			{
				CGameLevelLoader::GetInstance().ChangeLevel(new DummyLevel());
				break;
			}
		case ID_GAMEHOWTO:
			{
				CGameLevelLoader::GetInstance().ChangeLevel(new DummyLevel());
				break;
			}
		case ID_GAMERANK:
			{
				CGameLevelLoader::GetInstance().ChangeLevel(new DummyLevel());
				break;
			}
		case ID_GAMESHUTDOWN:
			{
				PostQuitMessage(0);
				break;
			}
		}

		spaceStatus = false;
	} 
}

bool LobbyLevel::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{	
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();

	m_Camera.Render();

	m_BackgroundBitmap.Render(device, screenWidth, screenHeight, 0, 0);
	instance.Render(device, m_BackgroundBitmap.GetIndexCount(), worldMatrix,
		m_Camera.GetViewMatrix(), orthMatrix, m_BackgroundBitmap.GetTexture());

	for (auto& button : m_button)
	{
		button.Render(device, screenWidth, screenHeight);

		instance.Render(device, button.GetIndexCount(), worldMatrix,
			m_Camera.GetViewMatrix(), orthMatrix, button.GetTexture());
	}

	return true;
}

void LobbyLevel::onStart()
{
}

void LobbyLevel::onEnd()
{
}
