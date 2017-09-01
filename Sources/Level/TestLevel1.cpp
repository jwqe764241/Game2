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
	WINDOWSIZE size = CGameApp::GetInstance().GetWindowSize();

	EnvironmentList.reserve(20);
	ActorList.reserve(20);
	ToolList.reserve(6);
	DeerList.reserve(10);

	LevelBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/background.png", 7680, 4320);
	LevelSize.top = 0;
	LevelSize.bottom = 4320;
	LevelSize.left = 0;
	LevelSize.right = 7680;

	//플레이어 생성
	GamePlayer = dynamic_cast<Player*>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_PLAYER, 576, 256));
	GamePlayer->SetPositionLimit(&LevelSize);
	GamePlayer->SetPosition({ 1920 / 2, 1080 / 2 });

	Camera.SetPosition(0.0f, 0.0f, -10.0f);

	Cursor.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/cursor.dds", 50, 50);
	PlayerUI.Initialize(CGameApp::GetInstance().GetDevice(), &GamePlayer->GetToolList(), &GamePlayer->GetItemList());

	for (int i = 0; i < g_PreDefinedToolAmount; ++i)
	{
		ToolList.push_back(new Tool(g_ToolList[i].tool_name, i));
		ToolList[i]->Initialize(CGameApp::GetInstance().GetDevice(), g_ToolList[i].resource_path, 50, 50, toolsPosition[i]);
	}

	for (int i = 0; i < 8; ++i)
	{
		if (i >= 0 && i <= 2)
		{
			EnvironmentList.push_back(new Rock());
		}
		else if (i == 3)
		{
			EnvironmentList.push_back(new Pond());
		}
		else if (i >= 4 && i <= 7)
		{
			EnvironmentList.push_back(new Tree());
		}

		EnvironmentList[i]->Load(CGameApp::GetInstance().GetDevice(), environmentList[i].filePath, environmentList[i].width, environmentList[i].height, environmentList[i].x, environmentList[i].y);
	}

	for (int i = 0; i < 5; ++i)
	{
		DeerList.push_back(new Deer());
		DeerList[i]->Load(CGameApp::GetInstance().GetDevice(), L"../Resources/deer.png", 96, 128, deerPosition[i].x, deerPosition[i].y);
	}

	onStart();

	return true;
}

void TestLevel1::Unload()
{
	LevelBitmap.Release();
	Cursor.Release();

	ReleaseList(EnvironmentList);
	ReleaseList(ActorList);
	ReleaseList(ToolList);
	ReleaseList(DeerList);

	Utils::Release(&GamePlayer);
}

void TestLevel1::Update(float dt)
{
	for (auto& target : ActorList)
	{
		target->Update(dt);
	}

	D3DXVECTOR2 pos = GamePlayer->GetPosition();
	WINDOWSIZE size = CGameApp::GetInstance().GetWindowSize();
	float halfWidth = size.width / 2;
	float halfHeight = size.height / 2;
	//고치기

	int xOffset = pos.x / 1920;
	int yOffset = pos.y / 1080;

	Camera.SetPosition(cameraPosX[xOffset], cameraPosY[yOffset], -10.f);

	for (auto itor = EnvironmentList.begin(); itor != EnvironmentList.end(); ++itor)
	{
		(*itor)->Update(dt);

		if (GameInput2::GetInstance().IsPressed(VK_SPACE))
		{
			if ((*itor)->CheckCollision(GamePlayer) && (*itor)->CheckTool(GamePlayer)) {
				(*itor)->OnAction(GamePlayer, dt);
				break;
			}
		}
		else
		{
			(*itor)->RefreshCooldown();
		}
	}

	for (auto itor = DeerList.begin(); itor != DeerList.end(); ++itor)
	{
		(*itor)->Update(dt);

		if (GameInput2::GetInstance().IsPressed(VK_SPACE))
		{

			if ((*itor)->CheckCollision(GamePlayer) && (*itor)->CheckTool(GamePlayer)) {
				(*itor)->OnAction(GamePlayer, dt);
				break;
			}
		}
		else
		{
			(*itor)->RefreshCooldown();
		}
	}

	for (auto itor = ToolList.begin(); itor != ToolList.end();)
	{
		bool collisionResult = Utils::CheckCollision(
			Utils::RECT_F{ pos.x, pos.y, pos.x + GamePlayer->GetSprite()->GetFrameWidth(), pos.y + GamePlayer->GetSprite()->GetFrameHeight() },
			Utils::RECT_F{ static_cast<float>((*itor)->GetPosition().x), static_cast<float>((*itor)->GetPosition().y),
			static_cast<float>((*itor)->GetPosition().x) + 50, static_cast<float>((*itor)->GetPosition().y) + 50 }
		);

		if (collisionResult)
		{
			if (itor != ToolList.end())
			{
				GamePlayer->AddTool((*itor));
				itor = ToolList.erase(itor);

				break;
			}
		}
		
		++itor;
	}

	GamePlayer->Update(dt);
	PlayerUI.Update(GamePlayer->GetHealth(),
		GamePlayer->GetWaterValue(),
		GamePlayer->GetFoodValue(),
		GamePlayer->GetSleepValue());

	int overCondition = 0;

	if (GamePlayer->GetHealth() <= 0)
	{
		overCondition++;
	}
	if (GamePlayer->GetWaterValue() <= 0)
	{
		overCondition++;
	}
	if (GamePlayer->GetFoodValue() <= 0)
	{
		overCondition++;
	}
	if (GamePlayer->GetSleepValue() <= 0)
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
	WINDOWSIZE size         = CGameApp::GetInstance().GetWindowSize();
	D3DXVECTOR3 cameraPos   = Camera.GetPosition();
	POINT pos               = GameInput2::GetInstance().GetMousePosition();

	Camera.Render();

	LevelBitmap.Render(device, screenWidth, screenHeight, 0, 0);
	instance.Render(device, LevelBitmap.GetIndexCount(), worldMatrix, 
		Camera.GetViewMatrix(), orthMatrix, LevelBitmap.GetTexture());

	/*
		렌더 리스트에 등록된 스프라이트 렌더
	*/
	RenderList(EnvironmentList, device, worldMatrix, orthMatrix, instance, screenWidth, screenHeight);

	RenderList(DeerList, device, worldMatrix, orthMatrix, instance, screenWidth, screenHeight);

	GamePlayer->Render(device, screenWidth, screenHeight);
	instance.Render(device, GamePlayer->GetIndexCount(), worldMatrix,
		Camera.GetViewMatrix(), orthMatrix, GamePlayer->GetTexture());

	RenderList(ToolList, device, worldMatrix, orthMatrix, instance, screenWidth, screenHeight);

	PlayerUI.Render(device, screenWidth, screenHeight, worldMatrix, Camera.GetViewMatrix(),
		orthMatrix, cameraPos, instance);

	Cursor.Render(device, size.width, size.height, pos.x + cameraPos.x, pos.y - cameraPos.y);

	instance.Render(device, Cursor.GetIndexCount(), worldMatrix, 
		Camera.GetViewMatrix(), orthMatrix, Cursor.GetTexture());

	return true;
}

void TestLevel1::onStart()
{
	Tool* spoon = new Tool(L"spoon", 5);
	spoon->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/spoon.png", 50, 50, toolsPosition[0]);
	GamePlayer->AddTool(spoon);
}

void TestLevel1::onEnd()
{

}

void TestLevel1::onGameOver()
{
	CGameLevelLoader::GetInstance().ChangeLevel(new GameOverLevel());
}




template<typename T>
void TestLevel1::ReleaseList(std::vector<T *>& list)
{
	for(T*& ptr : list)
	{
		Utils::Release(&ptr);
	}
}

template<typename T>
void TestLevel1::RenderList(std::vector<T *>& list, ID3D10Device* device, D3DXMATRIX worldMatrix, D3DXMATRIX projectionMatrix, TextureShader& shaderInstance, int screenWidth, int screenHeight)
{
	for(T*& ptr : list)
	{
		ptr->Render(device, screenWidth, screenHeight);

		shaderInstance.Render(device, ptr->GetIndexCount(), worldMatrix,
			Camera.GetViewMatrix(), projectionMatrix, ptr->GetTexture());
	}
}