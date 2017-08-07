#include <Sources/Level/TestLevel1.h>

TestLevel1::TestLevel1()
{

}

TestLevel1::~TestLevel1()
{

}

bool TestLevel1::Load()
{
	m_EnvironmentList.reserve(20);
	m_ActorList.reserve(20);

	m_LevelSize.top = 0;
	m_LevelSize.bottom = 1000;
	m_LevelSize.left = 0;
	m_LevelSize.right = 1000;

	/*
		테스트 전용으로 덤불 에셋 10개 추가
	*/

	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	m_EnvironmentList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));

	for (int i = 0; i < 10; ++i)
	{
		m_EnvironmentList[i]->SetPosition(m_PosList[i]);
	}
	/*
		캐스트는 옳은 방법이 아닌 것 같다; -> 뭐래냐? 나 미친거니.. 이 캐스팅은 봐주자...
		어떻게 다른 방법이 있으랴 
	*/
	m_ActorList.push_back(dynamic_cast<ICharacter *>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_ENEMY1, 576, 256)));
	m_ActorList.push_back(dynamic_cast<ICharacter *>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_ENEMY1, 576, 256)));
	m_ActorList.push_back(dynamic_cast<ICharacter *>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_ENEMY1, 576, 256)));
	m_ActorList.push_back(dynamic_cast<ICharacter *>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_ENEMY1, 576, 256)));
	m_ActorList.push_back(dynamic_cast<ICharacter *>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_ENEMY1, 576, 256)));

	for (int i = 0; i < 5; ++i)
	{
		m_ActorList[i]->SetPosition(m_SpawnPoint[i]);
	}

	//플레이어 생성
	m_Player = dynamic_cast<Player*>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_PLAYER, 576, 256));
	//m_Camera.SetPosition(0.0f, 0.0f, -1.0f);

	WindowSize size = CGameApp::GetInstance().GetWindowSize();
	m_Camera.SetPosition(m_LevelSize.left,
		(m_Player->GetPosition().y - size.height + m_Player->GetSprite()->GetFrameHeight() + 50) * -1, -10.0f);
	
	m_Player->SetPositionLimit(&m_LevelSize);

	onStart();

	return true;
}

void TestLevel1::Unload()
{
	for (auto target : m_EnvironmentList)
	{
		target->Release();
	}

	for (auto target : m_ActorList)
	{
		target->Release();
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
	if (pos.x + m_Player->GetSprite()->GetFrameWidth() >= (halfWidth) && pos.x <= m_LevelSize.right - (halfWidth))
	{
		m_Camera.SetPosition(pos.x - ((size.width / 2) - m_Player->GetSprite()->GetFrameWidth()), 
			(pos.y - size.height + m_Player->GetSprite()->GetFrameHeight() + 50) * -1 , -10.0f);
	}

	m_Player->Update(dt);

}

bool TestLevel1::Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	m_Camera.Render();

	/*
		테스트 전용이므로 플레이어 먼저 렌더
	*/
	m_Player->Render(deviceContext, screenWidth, screenHeight);
	TextureShader::GetInstance().Render(deviceContext, m_Player->GetIndexCount(), CGameApp::GetInstance().GetWorldMatrix(), m_Camera.GetViewMatrix(),
		CGameApp::GetInstance().GetorthogonalMatrix(), m_Player->GetTexture());

	//for(auto target : m_ActorList)
	//{
	//	target->Render(deviceContext, screenWidth, screenHeight);

	//	TextureShader::GetInstance().Render(deviceContext, target->GetIndexCount(), CGameApp::GetInstance().GetWorldMatrix(), m_Camera.GetViewMatrix(),
	//		CGameApp::GetInstance().GetorthogonalMatrix(), target->GetTexture());
	//}

	/*
		렌더 리스트에 등록된 스프라이트 렌더
	*/
	for (auto target : m_EnvironmentList)
	{
		target->Render(deviceContext, screenWidth, screenHeight);

		TextureShader::GetInstance().Render(deviceContext, target->GetIndexCount(), CGameApp::GetInstance().GetWorldMatrix(), m_Camera.GetViewMatrix(),
			CGameApp::GetInstance().GetorthogonalMatrix(), target->GetTexture());
	}


	return true;
}

void TestLevel1::onStart()
{

}

void TestLevel1::onEnd()
{

}