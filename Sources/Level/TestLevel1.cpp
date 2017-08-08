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

	bool a = m_LevelBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/background.png", 7680, 4320);

	m_LevelSize.top = 0;
	m_LevelSize.bottom = 4320;
	m_LevelSize.left = 0;
	m_LevelSize.right = 7680;

	//플레이어 생성
	m_Player = dynamic_cast<Player*>(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_PLAYER, 576, 256));
	//m_Camera.SetPosition(0.0f, 0.0f, -1.0f);

	WindowSize size = CGameApp::GetInstance().GetWindowSize();
	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);
	
	m_Player->SetPositionLimit(&m_LevelSize);

	onStart();

	return true;
}

void TestLevel1::Unload()
{
	m_LevelBitmap.Release();

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
	float halfHeight = size.height / 2;
	//고치기
	
	if (pos.x + m_Player->GetSprite()->GetFrameWidth() >= (halfWidth) && pos.x <= m_LevelSize.right - (halfWidth) ||
		pos.y >= (halfHeight) && pos.y <= m_LevelSize.bottom - (halfHeight))
	{
		m_Camera.SetPosition(pos.x - ((size.width / 2) - m_Player->GetSprite()->GetFrameWidth()),
			(pos.y - ((size.height / 2) - m_Player->GetSprite()->GetFrameHeight())) * -1, -10.0f);
	}

	//float x = pos.x - ((size.width / 2) - m_Player->GetSprite()->GetFrameWidth());
	//float y = (pos.y - ((size.height / 2) - m_Player->GetSprite()->GetFrameHeight())) * -1;

	m_Player->Update(dt);
}

bool TestLevel1::Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight)
{
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();

	m_Camera.Render();

	m_LevelBitmap.Render(deviceContext, screenWidth, screenHeight, 0, 0);
	instance.Render(deviceContext, m_LevelBitmap.GetIndexCount(), worldMatrix, 
		m_Camera.GetViewMatrix(), orthMatrix, m_LevelBitmap.GetTexture());

	/*
		테스트 전용이므로 플레이어 먼저 렌더
	*/
	m_Player->Render(deviceContext, screenWidth, screenHeight);
	instance.Render(deviceContext, m_Player->GetIndexCount(), worldMatrix, 
		m_Camera.GetViewMatrix(), orthMatrix, m_Player->GetTexture());

	/*
		렌더 리스트에 등록된 스프라이트 렌더
	*/
	for (auto target : m_EnvironmentList)
	{
		target->Render(deviceContext, screenWidth, screenHeight);

		instance.Render(deviceContext, target->GetIndexCount(), worldMatrix, 
			m_Camera.GetViewMatrix(), orthMatrix, target->GetTexture());
	}

	return true;
}

void TestLevel1::onStart()
{

}

void TestLevel1::onEnd()
{

}