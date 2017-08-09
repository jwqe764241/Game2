#include <Sources/Level/LobbyLevel.h>

LobbyLevel::LobbyLevel()
{

}

LobbyLevel::~LobbyLevel()
{

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
				MessageBox(NULL, L"1", L"dsfsdf", MB_OK);
				break;
			}
		case ID_GAMEHOWTO:
			{
				MessageBox(NULL, L"2", L"dsfsdf", MB_OK);
				break;
			}
		case ID_GAMERANK:
			{
				MessageBox(NULL, L"3", L"dsfsdf", MB_OK);
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

bool LobbyLevel::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{	
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();
	WindowSize size = CGameApp::GetInstance().GetWindowSize();

	m_Camera.Render();

	m_BackgroundBitmap.Render(deviceContext, screenWidth, screenHeight, 0, 0);
	instance.Render(deviceContext, m_BackgroundBitmap.GetIndexCount(), worldMatrix,
		m_Camera.GetViewMatrix(), orthMatrix, m_BackgroundBitmap.GetTexture());

	for (auto& button : m_button)
	{
		button.Render(deviceContext, screenWidth, screenHeight);

		instance.Render(deviceContext, button.GetIndexCount(), worldMatrix,
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
