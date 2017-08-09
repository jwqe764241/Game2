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

	/*
	if (input.IsPressed(VK_UP))
	{
		std::cout << "sibal" << input.IsReleased(VK_UP) << std::endl;
		if (input.IsReleased(VK_UP) && (currSelection > 0))
		{
			m_button[currSelection].Update(false);
			m_button[--currSelection].Update(true);
		}
	}
	else if (input.IsPressed(VK_DOWN))
	{
		if (input.IsReleased(VK_DOWN) && (currSelection < 4))
		{
			m_button[currSelection].Update(false);
			m_button[++currSelection].Update(true);
		}
	}
	*/
}

bool LobbyLevel::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{	
	TextureShader& instance = TextureShader::GetInstance();
	D3DXMATRIX worldMatrix = CGameApp::GetInstance().GetWorldMatrix();
	D3DXMATRIX orthMatrix = CGameApp::GetInstance().GetorthogonalMatrix();
	WindowSize size = CGameApp::GetInstance().GetWindowSize();

	m_Camera.Render();

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
