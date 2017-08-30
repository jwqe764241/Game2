#include <Sources/GUI/Button.h>

Button::Button()
{

}

Button::~Button()
{
	Release();
}

void Button::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
	m_statusBitmap[0] = new GameBitmap();
	m_statusBitmap[1] = new GameBitmap();
	
	m_statusBitmap[0]->Initialize(device, L"../Resources/Button_NotSelected.png", bitmapWidth, bitmapHeight);
	m_statusBitmap[1]->Initialize(device, L"../Resources/Button_Selected.png", bitmapWidth, bitmapHeight);

	m_pCurrBitmap = m_statusBitmap[0];

	m_pos = { 0, 0 };
}

void Button::Release()
{
	for (auto target : m_statusBitmap)
	{
		Utils::Release(&target);
	}
}

void Button::Reset()
{
}

void Button::Update(float dt)
{
	
}

void Button::Update(bool selected)
{
	m_pCurrBitmap = m_statusBitmap[selected];
}

void Button::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	m_pCurrBitmap->Render(device, screenWidth, screenHeight, m_pos.x, m_pos.y);
}

int Button::GetIndexCount()
{
	return m_pCurrBitmap->GetIndexCount();
}

ID3D10ShaderResourceView * Button::GetTexture()
{
	return m_pCurrBitmap->GetTexture();
}

D3DXVECTOR2 Button::GetPosition() const
{
	return m_pos;
}

void Button::SetPosition(const D3DXVECTOR2 pos)
{
	m_pos = pos;
}
