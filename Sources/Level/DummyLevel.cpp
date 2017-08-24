#include <Sources/Level/DummyLevel.h>

DummyLevel::DummyLevel()
{

}

DummyLevel::~DummyLevel()
{
	Unload();
}

bool DummyLevel::Load()
{
	m_Camera.SetPosition(0.0f, 0.0f, -10.0f);
	
	m_BackgroundBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/DummyLevel.png", 1920, 1080);
	
	m_BackButton.Load(CGameApp::GetInstance().GetDevice(), 300, 150);
	m_BackButton.SetPosition(D3DXVECTOR2{ 1520, 900 });
	m_BackButton.Update(true);

	return true;
}

void DummyLevel::Unload()
{
	m_BackgroundBitmap.Release();

	m_BackButton.Release();
}

void DummyLevel::Update(float dt)
{
	GameInput2& input = GameInput2::GetInstance();
	static bool spaceStatus = false;

	if (input.IsPressed(VK_SPACE))
	{
		spaceStatus = true;
		return;
	}

	if (spaceStatus)
	{
		CGameLevelLoader::GetInstance().ChangeLevel(new LobbyLevel());
		spaceStatus = false;
	}
}

bool DummyLevel::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();

	m_Camera.Render();

	m_BackgroundBitmap.Render(deviceContext, screenWidth, screenHeight, 0, 0);
	instance.Render(deviceContext, m_BackgroundBitmap.GetIndexCount(), worldMatrix,
		m_Camera.GetViewMatrix(), orthMatrix, m_BackgroundBitmap.GetTexture());

	m_BackButton.Render(deviceContext, screenWidth, screenHeight);
	instance.Render(deviceContext, m_BackButton.GetIndexCount(), worldMatrix,
		m_Camera.GetViewMatrix(), orthMatrix, m_BackButton.GetTexture());

	return true;
}

void DummyLevel::onStart()
{

}

void DummyLevel::onEnd()
{
}
