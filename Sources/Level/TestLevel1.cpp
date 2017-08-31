#include <Sources/Level/TestLevel1.h>

TestLevel1::TestLevel1()
{

}

TestLevel1::~TestLevel1()
{
	Unload();
}

bool TestLevel1::Load()
{
	WindowSize size = CGameApp::GetInstance().GetWindowSize();

	m_EnvironmentList.reserve(20);
	m_ActorList.reserve(20);
	m_Tools.reserve(6);
	m_DeerList.reserve(10);

	m_LevelBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/background.png", 7680, 4320);
	m_LevelSize.top = 0;
	m_LevelSize.bottom = 4320;
	m_LevelSize.left = 0;
	m_LevelSize.right = 7680;

	//플레이어 생성
	m_Player = dynamic_cast<Player*>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_PLAYER, 576, 256));
	m_Player->SetPositionLimit(&m_LevelSize);
	m_Player->SetPosition({ 1920 / 2, 1080 / 2 });

	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);

	m_Cursor.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/cursor.dds", 50, 50);
	m_PlayerUI.Initialize(CGameApp::GetInstance().GetDevice(), &m_Player->GetToolList(), &m_Player->GetItemList());

	for (int i = 0; i < g_PreDefinedToolAmount; ++i)
	{
		m_Tools.push_back(new Tool(g_ToolList[i].tool_name, i));
		m_Tools[i]->Initialize(CGameApp::GetInstance().GetDevice(), g_ToolList[i].resource_path, 50, 50, toolsPosition[i]);
	}

	for (int i = 0; i < 8; ++i)
	{
		if (i >= 0 && i <= 2)
		{
			m_EnvironmentList.push_back(new Rock());
		}
		else if (i == 3)
		{
			m_EnvironmentList.push_back(new Pond());
		}
		else if (i >= 4 && i <= 7)
		{
			m_EnvironmentList.push_back(new Tree());
		}

		m_EnvironmentList[i]->Load(CGameApp::GetInstance().GetDevice(), environmentList[i].filePath, environmentList[i].width, environmentList[i].height, environmentList[i].x, environmentList[i].y);
	}

	for (int i = 0; i < 5; ++i)
	{
		m_DeerList.push_back(new Deer());
		m_DeerList[i]->Load(CGameApp::GetInstance().GetDevice(), L"../Resources/deer.png", 96, 128, deerPosition[i].x, deerPosition[i].y);
	}

	onStart();

	return true;
}

void TestLevel1::Unload()
{
	m_LevelBitmap.Release();
	m_Cursor.Release();

	for (auto& target : m_EnvironmentList)
	{
		target->Release();
	}

	for (auto& target : m_ActorList)
	{
		target->Release();
	}

	for (auto& target : m_Tools)
	{
		Utils::Release(&target);
	}

	for (auto& target : m_DeerList)
	{
		Utils::Release(&target);
	}

	Utils::Release(&m_Player);
}

void TestLevel1::Update(float dt)
{
	for (auto& target : m_EnvironmentList)
	{
		target->Update(dt);
	}

	for (auto& target : m_ActorList)
	{
		target->Update(dt);
	}

	D3DXVECTOR2 pos = m_Player->GetPosition();
	WindowSize size = CGameApp::GetInstance().GetWindowSize();
	float halfWidth = size.width / 2;
	float halfHeight = size.height / 2;
	//고치기

	int xOffset = pos.x / 1920;
	int yOffset = pos.y / 1080;

	m_Camera.SetPosition(cameraPosX[xOffset], cameraPosY[yOffset], -10.f);

	for (auto itor = m_EnvironmentList.begin(); itor != m_EnvironmentList.end(); ++itor)
	{
		if (GameInput2::GetInstance().IsPressed(VK_SPACE))
		{
			if ((*itor)->CheckCollision(m_Player) && (*itor)->CheckTool(m_Player)) {
				(*itor)->OnAction(m_Player, dt);
				break;
			}
		}
		else
		{
			(*itor)->RefreshCooldown();
		}
	}

	for (auto itor = m_DeerList.begin(); itor != m_DeerList.end(); ++itor)
	{
		(*itor)->Update(dt);

		if (GameInput2::GetInstance().IsPressed(VK_SPACE))
		{

			if ((*itor)->CheckCollision(m_Player) && (*itor)->CheckTool(m_Player)) {
				(*itor)->OnAction(m_Player, dt);
				break;
			}
		}
		else
		{
			(*itor)->RefreshCooldown();
		}
	}

	for (auto itor = m_Tools.begin(); itor != m_Tools.end();)
	{
		bool collisionResult = Utils::CheckCollision(
			Utils::RECT_F{ pos.x, pos.y, pos.x + m_Player->GetSprite()->GetFrameWidth(), pos.y + m_Player->GetSprite()->GetFrameHeight() },
			Utils::RECT_F{ static_cast<float>((*itor)->GetPosition().x), static_cast<float>((*itor)->GetPosition().y),
			static_cast<float>((*itor)->GetPosition().x) + 50, static_cast<float>((*itor)->GetPosition().y) + 50 }
		);


		if (collisionResult)
		{
			if (itor != m_Tools.end())
			{
				m_Player->AddTool((*itor));
				itor = m_Tools.erase(itor);

				break;
			}
		}
		
		++itor;
	}

	m_Player->Update(dt);
	m_PlayerUI.Update(m_Player->GetHealth(),
		m_Player->GetWaterValue(),
		m_Player->GetFoodValue(),
		m_Player->GetSleepValue());

	int overCondition = 0;

	if (m_Player->GetHealth() <= 0)
	{
		overCondition++;
	}
	if (m_Player->GetWaterValue() <= 0)
	{
		overCondition++;
	}
	if (m_Player->GetFoodValue() <= 0)
	{
		overCondition++;
	}
	if (m_Player->GetSleepValue() <= 0)
	{
		overCondition++;
	}

	if (overCondition >= 2)
	{
		onGameOver();
	}
}

bool TestLevel1::Render(ID3D10Device* device, int screenWidth, int screenHeight)
{
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix  = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix   = CGameApp::GetInstance().GetorthogonalMatrix();
	WindowSize size         = CGameApp::GetInstance().GetWindowSize();
	D3DXVECTOR3 cameraPos   = m_Camera.GetPosition();
	POINT pos               = GameInput2::GetInstance().GetMousePosition();

	m_Camera.Render();

	m_LevelBitmap.Render(device, screenWidth, screenHeight, 0, 0);
	instance.Render(device, m_LevelBitmap.GetIndexCount(), worldMatrix, 
		m_Camera.GetViewMatrix(), orthMatrix, m_LevelBitmap.GetTexture());

	/*
		렌더 리스트에 등록된 스프라이트 렌더
	*/
	for (auto& target : m_EnvironmentList)
	{
		target->Render(device, screenWidth, screenHeight);

		instance.Render(device, target->GetIndexCount(), worldMatrix, 
			m_Camera.GetViewMatrix(), orthMatrix, target->GetTexture());
	}

	for (auto& target : m_DeerList)
	{
		target->Render(device, screenWidth, screenHeight);

		instance.Render(device, target->GetIndexCount(), worldMatrix,
			m_Camera.GetViewMatrix(), orthMatrix, target->GetTexture());
	}

	m_Player->Render(device, screenWidth, screenHeight);
	instance.Render(device, m_Player->GetIndexCount(), worldMatrix,
		m_Camera.GetViewMatrix(), orthMatrix, m_Player->GetTexture());

	for (auto& target : m_Tools)
	{
		target->Render(device, screenWidth, screenHeight);

		instance.Render(device, target->GetIndexCount(), worldMatrix,
			m_Camera.GetViewMatrix(), orthMatrix, target->GetTexture());
	}

	m_PlayerUI.Render(device, screenWidth, screenHeight, worldMatrix, m_Camera.GetViewMatrix(),
		orthMatrix, cameraPos, instance);

	m_Cursor.Render(device, size.width, size.height, pos.x + cameraPos.x, pos.y - cameraPos.y);

	instance.Render(device, m_Cursor.GetIndexCount(), worldMatrix, 
		m_Camera.GetViewMatrix(), orthMatrix, m_Cursor.GetTexture());

	return true;
}

void TestLevel1::onStart()
{
	Tool* spoon = new Tool(L"spoon", 5);
	spoon->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/spoon.png", 50, 50, toolsPosition[0]);
	m_Player->AddTool(spoon);
}

void TestLevel1::onEnd()
{

}

void TestLevel1::onGameOver()
{
	CGameLevelLoader::GetInstance().ChangeLevel(new GameOverLevel());
}