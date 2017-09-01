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
	Camera.SetPosition(0.0f, 0.0f, -10.0f);
	
	LevelBitmap.Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/DummyLevel.png", 1920, 1080);
	
	MenuButton.Load(CGameApp::GetInstance().GetDevice(), 300, 150);
	MenuButton.SetPosition(D3DXVECTOR2{ 1520, 900 });
	MenuButton.Update(true);

	return true;
}

void DummyLevel::Unload()
{
	LevelBitmap.Release();

	MenuButton.Release();
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

bool DummyLevel::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();

	Camera.Render();

	LevelBitmap.Render(device, screenWidth, screenHeight, 0, 0);
	instance.Render(device, LevelBitmap.GetIndexCount(), worldMatrix,
		Camera.GetViewMatrix(), orthMatrix, LevelBitmap.GetTexture());

	MenuButton.Render(device, screenWidth, screenHeight);
	instance.Render(device, MenuButton.GetIndexCount(), worldMatrix,
		Camera.GetViewMatrix(), orthMatrix, MenuButton.GetTexture());

	return true;
}

void DummyLevel::onStart()
{

}

void DummyLevel::onEnd()
{
}
