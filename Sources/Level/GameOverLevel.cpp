#include <Sources/Level/GameOverLevel.h>

bool GameOverLevel::Load()
{
	LevelBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/DummyLevel.png", 1920, 1080);

	MenuButtons[0].Load(CGameApp::GetInstance().GetDevice(), 300, 150);
	MenuButtons[1].Load(CGameApp::GetInstance().GetDevice(), 300, 150);

	MenuButtons[0].SetPosition(D3DXVECTOR2{ 1595, 730 });
	MenuButtons[1].SetPosition(D3DXVECTOR2{ 1595, 905 });

	MenuButtons[0].Update(true);

	writer_48size.Initialize(CGameApp::GetInstance().GetDevice(), 55, 0);

	Camera.SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

void GameOverLevel::Unload()
{
	LevelBitmap.Release();

	for (auto& target : MenuButtons)
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

	if (upStatus && (CurrentSelection > 0))
	{
		MenuButtons[CurrentSelection].Update(false);
		MenuButtons[--CurrentSelection].Update(true);
		upStatus = false;
	}
	else if (downStatus && (CurrentSelection < 1))
 	{
		MenuButtons[CurrentSelection].Update(false);
		MenuButtons[++CurrentSelection].Update(true);
		downStatus = false;
	}
	else if (spaceStatus)
	{
		switch (CurrentSelection)
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

	Camera.Render();

	LevelBitmap.Render(device, screenWidth, screenHeight, 0, 0);
	instance.Render(device, LevelBitmap.GetIndexCount(), worldMatrix,
		Camera.GetViewMatrix(), orthMatrix, LevelBitmap.GetTexture());

	for (auto& button : MenuButtons)
	{
		button.Render(device, screenWidth, screenHeight);

		instance.Render(device, button.GetIndexCount(), worldMatrix,
			Camera.GetViewMatrix(), orthMatrix, button.GetTexture());
	}

	writer_48size.DrawString(L"재시작",
		RECT{ (LONG)MenuButtons[0].GetPosition().x, (LONG)MenuButtons[0].GetPosition().y,
		(LONG)MenuButtons[0].GetPosition().x + 300,(LONG)MenuButtons[0].GetPosition().y + 150},
		DT_CENTER | DT_VCENTER, D3DXCOLOR{1.0f, 1.0f, 1.0f, 1.0f}
	);

	writer_48size.DrawString(L"나가기",
		RECT{(LONG)MenuButtons[1].GetPosition().x,  (LONG)MenuButtons[1].GetPosition().y,
		(LONG)MenuButtons[1].GetPosition().x + 300, (LONG)MenuButtons[1].GetPosition().y + 150 },
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
