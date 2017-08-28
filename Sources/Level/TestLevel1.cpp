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

	m_LevelBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/background.png", 7680, 4320);
	m_LevelSize.top = 0;
	m_LevelSize.bottom = 4320;
	m_LevelSize.left = 0;
	m_LevelSize.right = 7680;

	//플레이어 생성
	m_Player = dynamic_cast<Player*>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_PLAYER, 576, 256));
	m_Player->SetPositionLimit(&m_LevelSize);

	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);

	m_Cursor.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/cursor.png", 50, 50);
	m_PlayerUI.Initialize(&m_Player->GetToolList());

	//위험
	m_Tools.push_back(new Tool(L"saw", 1));
	m_Tools.push_back(new Tool(L"pickaxe", 2));
	m_Tools.push_back(new Tool(L"hammer", 3));
	m_Tools.push_back(new Tool(L"bottle", 4));
	m_Tools.push_back(new Tool(L"axe", 5));

	m_Tools[0]->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/saw.png", 50, 50, toolsPosition[0]);
	m_Tools[1]->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/pickaxe.png", 50, 50, toolsPosition[1]);
	m_Tools[2]->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/hammer.png", 50, 50, toolsPosition[2]);
	m_Tools[3]->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/bottle.png", 50, 50, toolsPosition[3]);
	m_Tools[4]->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/axe.png", 50, 50, toolsPosition[4]);

	Tool* spoon = new Tool(L"spoon", 0);
	spoon->Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/tools/spoon.png", 50, 50, toolsPosition[0]);
	m_Player->AddTool(spoon);

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

	Utils::Release(&m_Player);
}

void TestLevel1::Update(float dt)
{
	for (auto target : m_EnvironmentList)
	{
		target->Update(dt);
	}

	//for (auto target : m_ActorList)
	//{
	//	target->Update(dt);
	//}

	/*
		스페이스바 누를 시에 카메라 트레킹
		위치 기준은 Player가 화면 중앙에 오도록 함
	*/
	//GameInput 버전
	//if (GameInput::GetInstance().IsKeyPressed(DIK_SPACE))
	//{
	//	D3DXVECTOR2 pos = m_Player->GetPosition();
	//	float width  = CGameApp::GetInstance().GetWindowSize().width;
	//	float height = CGameApp::GetInstance().GetWindowSize().height;

	//	m_Camera.SetPosition(pos.x - ((width / 2) - m_Player->GetSprite()->GetFrameWidth()), 
	//		(pos.y - ((height / 2) - m_Player->GetSprite()->GetFrameHeight())) * -1 , -10.0f);
	//}

	//GameInput2 버전

	D3DXVECTOR2 pos = m_Player->GetPosition();
	WindowSize size = CGameApp::GetInstance().GetWindowSize();
	float halfWidth = size.width / 2;
	float halfHeight = size.height / 2;
	//고치기

	int xOffset = pos.x / 1920;
	int yOffset = pos.y / 1080;

	m_Camera.SetPosition(cameraPosX[xOffset], cameraPosY[yOffset], -10.f);

	//if (pos.x + m_Player->GetSprite()->GetFrameWidth() >= (halfWidth) && pos.x <= m_LevelSize.right - (halfWidth) ||
	//	pos.y >= (halfHeight) && pos.y <= m_LevelSize.bottom - (halfHeight))
	//{
	//	m_Camera.SetPosition(pos.x - ((size.width / 2) - m_Player->GetSprite()->GetFrameWidth()),
	//		(pos.y - ((size.height / 2) - m_Player->GetSprite()->GetFrameHeight())) * -1, -10.0f);
	//}

	//float x = pos.x - ((size.width / 2) - m_Player->GetSprite()->GetFrameWidth());
	//float y = (pos.y - ((size.height / 2) - m_Player->GetSprite()->GetFrameHeight())) * -1;

	m_Player->Update(dt);
	m_PlayerUI.Update(m_Player->GetHealth(),
		m_Player->GetWaterValue(),
		m_Player->GetFoodValue(),
		m_Player->GetSleepValue());

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
		
		itor++;
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
		테스트 전용이므로 플레이어 먼저 렌더
	*/
	m_Player->Render(device, screenWidth, screenHeight);
	instance.Render(device, m_Player->GetIndexCount(), worldMatrix, 
		m_Camera.GetViewMatrix(), orthMatrix, m_Player->GetTexture());

	/*
		렌더 리스트에 등록된 스프라이트 렌더
	*/
	for (auto& target : m_EnvironmentList)
	{
		target->Render(device, screenWidth, screenHeight);

		instance.Render(device, target->GetIndexCount(), worldMatrix, 
			m_Camera.GetViewMatrix(), orthMatrix, target->GetTexture());
	}

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

}

void TestLevel1::onEnd()
{

}

void TestLevel1::onGameOver()
{
	
}