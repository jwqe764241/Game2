#include <Sources/Level/TestLevel1.h>

TestLevel1::TestLevel1()
{

}

TestLevel1::~TestLevel1()
{

}

bool TestLevel1::Load()
{
	m_RenderList.reserve(20);

	m_RenderList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	//m_RenderList.push_back(CGameAssetLoader::GetInstance().LoadAsset(ID_ASSET_TESTASSET, 32, 32));
	return true;
}

void TestLevel1::Unload()
{
	for (auto target : m_RenderList)
	{
		target->Release();
	}
}

void TestLevel1::Update(float dt)
{
	for (std::vector<IRenderable*>::iterator idx = m_RenderList.begin(); idx != m_RenderList.end(); idx++)
	{
		(*idx)->Update(dt);
	}
}

bool TestLevel1::Render(ID3D11DeviceContext* deviceContext)
{
	for (std::vector<IRenderable*>::iterator idx = m_RenderList.begin(); idx != m_RenderList.end(); idx++)
	{
		(*idx)->Render(deviceContext);

		TextureShader::GetInstance().Render(deviceContext, (*idx)->GetIndexCount(), CGameApp::GetInstance().GetWorldMatrix(), CGameApp::GetInstance().m_Camera.GetViewMatrix(),
			CGameApp::GetInstance().GetorthogonalMatrix(), (*idx)->GetTexture());
	}

	return true;
}


void TestLevel1::onStart()
{

}

void TestLevel1::onEnd()
{

}