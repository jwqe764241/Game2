#include <Sources/Assets/StateUI.h>


#include <Sources/GameApp.h>

StateUI::StateUI()
{

}

StateUI::~StateUI()
{
	Release();
}

void StateUI::Initialize(std::vector<Tool *>* toolList)
{
	stateBar[0].Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/HealthState.png", 200, 50, 100, 100);
	stateBar[1].Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/WaterState.png", 200, 50, 100, 100);
	stateBar[2].Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/FoodState.png", 200, 50, 100, 100);
	stateBar[3].Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/SleepState.png", 200, 50, 100, 100);
	stateBar[4].Initialize(CGameApp::GetInstance().GetDevice(), L"../Resources/EmptyState.png", 200, 50, 100, 100);

	m_ToolList = toolList;
}

void StateUI::Release()
{
	for (auto& target : stateBar)
	{
		target.Release();
	}
}

void StateUI::Update(int healthValue, int waterValue, int foodValue, int sleepValue)
{
	stateBar[0].SetValue(healthValue);
	stateBar[1].SetValue(waterValue);
	stateBar[2].SetValue(foodValue);
	stateBar[3].SetValue(sleepValue);
}

void StateUI::Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight,
	D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthMatrix, D3DXVECTOR3 cameraPos, TextureShader& instance)
{
	for (int i = 0; i < 4; i++)
	{
		//ºó »óÅÂ ¸ÕÀú ·»´õ
		stateBar[4].Render(deviceContext, screenWidth, screenHeight, (cameraPos.x + 25) + (225 * i), (cameraPos.y * -1) + 25);
		instance.Render(deviceContext, stateBar[i].GetIndexCount(), worldMatrix,
			viewMatrix, orthMatrix, stateBar[4].GetTexture());

		//ÇöÀç »óÅÂ ·»´õ
		stateBar[i].Render(deviceContext, screenWidth, screenHeight, (cameraPos.x + 25) + (225 * i), (cameraPos.y * -1) + 25);
		instance.Render(deviceContext, stateBar[i].GetIndexCount(), worldMatrix,
			viewMatrix, orthMatrix, stateBar[i].GetTexture());
	}

	for (int i = 0; i < m_ToolList->size(); i++)
	{
		(*m_ToolList)[i]->Render(deviceContext, screenWidth, screenHeight, (cameraPos.x + 25) + (75 * i), (cameraPos.y * -1) + 1000);

		instance.Render(deviceContext, (*m_ToolList)[i]->GetIndexCount(), worldMatrix,
			viewMatrix, orthMatrix, (*m_ToolList)[i]->GetTexture());
	}
}