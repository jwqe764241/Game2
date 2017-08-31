#include <Sources/Level/GameOverLevel.h>

bool GameOverLevel::Load()
{
	m_Background.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/DummyLevel.png", 1920, 1080);

	m_Button[0].Load(CGameApp::GetInstance().GetDevice(), 300, 150);
	m_Button[1].Load(CGameApp::GetInstance().GetDevice(), 300, 150);

	m_Button[0].SetPosition(D3DXVECTOR2{ 1595, 730 });
	m_Button[1].SetPosition(D3DXVECTOR2{ 1595, 905 });

	m_Button[0].Update(true);

	writer_48size.Initialize(CGameApp::GetInstance().GetDevice(), 55, 0);

	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

void GameOverLevel::Unload()
{
	m_Background.Release();

	for (auto& target : m_Button)
	{
		target.Release();
	}
}

void GameOverLevel::Update(float dt)
{
	GameInput2& input = GameInput2::GetInstance();\
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

	if (input.IsPressed(VK_SPACE))
	{
		spaceStatus = true;
		return;
	}

	if (upStatus && (currSelection > 0))
	{
		m_Button[currSelection].Update(false);
		m_Button[--currSelection].Update(true);
		upStatus = false;
	}
	else if (downStatus && (currSelection < 1))
	{
		m_Button[currSelection].Update(false);
		m_Button[++currSelection].Update(true);
		downStatus = false;
	}
	else if (spaceStatus)
	{
		switch (currSelection)
		{
		case 0:
		{
			CGameLevelLoader::GetInstance().ChangeLevel(new TestLevel1());
			break;
		}
		case 1:
		{
			CGameLevelLoader::GetInstance().ChangeLevel(new DummyLevel());
			break;
		}
		}

		spaceStatus = false;
	}
}

bool GameOverLevel::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();

	m_Camera.Render();

	m_Background.Render(device, screenWidth, screenHeight, 0, 0);
	instance.Render(device, m_Background.GetIndexCount(), worldMatrix,
		m_Camera.GetViewMatrix(), orthMatrix, m_Background.GetTexture());

	for (auto& button : m_Button)
	{
		button.Render(device, screenWidth, screenHeight);

		instance.Render(device, button.GetIndexCount(), worldMatrix,
			m_Camera.GetViewMatrix(), orthMatrix, button.GetTexture());
	}

	writer_48size.DrawString(L"재시작",
		RECT{ (LONG)m_Button[0].GetPosition().x, (LONG)m_Button[0].GetPosition().y,
		(LONG)m_Button[0].GetPosition().x + 300,(LONG)m_Button[0].GetPosition().y + 150},
		DT_CENTER | DT_VCENTER, D3DXCOLOR{1.0f, 1.0f, 1.0f, 1.0f}
	);

	writer_48size.DrawString(L"나가기",
		RECT{(LONG)m_Button[1].GetPosition().x,  (LONG)m_Button[1].GetPosition().y,
		(LONG)m_Button[1].GetPosition().x + 300, (LONG)m_Button[1].GetPosition().y + 150 },
		DT_CENTER | DT_VCENTER, D3DXCOLOR{ 1.0f, 1.0f, 1.0f, 1.0f }
	);

	return true;
}

void GameOverLevel::onStart()
{
}

void GameOverLevel::onEnd()
{
}
