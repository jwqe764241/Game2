#include <Sources/Level/IntroLevel.h>

IntroLevel::IntroLevel()
{

}

IntroLevel::~IntroLevel()
{

}

bool IntroLevel::Load()
{
	m_Camera.SetPosition(0, 0, -10.f);

	return true;
}

void IntroLevel::Unload()
{
}

void IntroLevel::Update(float dt)
{
}

bool IntroLevel::Render(ID3D11DeviceContext * deviceContext, int screenWidth, int screenHeight)
{
	return false;
}

void IntroLevel::onStart()
{
}

void IntroLevel::onEnd()
{
}
