#include <Sources/GameTextWriter.h>

GameTextWriter::GameTextWriter()
{

}

GameTextWriter::~GameTextWriter()
{

}

bool GameTextWriter::Initialize(ID3D10Device* pDevice, int height, int width)
{
	HRESULT result;

	D3DX10_FONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(D3DX10_FONT_DESC));
		fontDesc.Height = height;
		fontDesc.Width = width;
		fontDesc.Weight = FW_NORMAL;
		fontDesc.MipLevels = 0;
		fontDesc.Italic = false;
		fontDesc.CharSet = D3DX10_DEFAULT;
		fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
		fontDesc.Quality = CLEARTYPE_NATURAL_QUALITY;
		fontDesc.PitchAndFamily = 0;
		wcscpy_s(fontDesc.FaceName, TEXT("¸¼Àº °íµñ"));

	result = D3DX10CreateFontIndirect(pDevice, &fontDesc, &Font);
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX10CreateSprite(pDevice, sizeof(pDevice), &Sprite);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void GameTextWriter::DrawString(wchar_t* string, RECT rect, UINT format, D3DXCOLOR color)
{
	Sprite->Begin(D3DX10_SPRITE_SAVE_STATE);

	Font->DrawTextW(nullptr, string, -1, &rect, format, color);

	Sprite->End();
}