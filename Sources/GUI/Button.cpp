#include <Sources/GUI/Button.h>

Button::Button()
{

}

Button::~Button()
{
	Release();
}

void Button::Load(ID3D10Device* device, int bitmapWidth, int bitmapHeight)
{
	StatusBitmaps[0].Initialize(device, L"../Resources/Button_NotSelected.png", bitmapWidth, bitmapHeight);
	StatusBitmaps[1].Initialize(device, L"../Resources/Button_Selected.png", bitmapWidth, bitmapHeight);

	CurrentBitmap = &StatusBitmaps[0];

	Position = { 0, 0 };
}

void Button::Release()
{
	StatusBitmaps[0].Release();
	StatusBitmaps[1].Release();
}

void Button::Reset()
{
}

void Button::Update(float dt)
{
	
}

void Button::Update(bool selected)
{
	CurrentBitmap = &StatusBitmaps[selected];
}

void Button::Render(ID3D10Device* device, int screenWidth, int screenHeight)
{
	CurrentBitmap->Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

int Button::GetIndexCount()
{
	return CurrentBitmap->GetIndexCount();
}

ID3D10ShaderResourceView* Button::GetTexture()
{
	return CurrentBitmap->GetTexture();
}

D3DXVECTOR2 Button::GetPosition() const
{
	return Position;
}

void Button::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}
