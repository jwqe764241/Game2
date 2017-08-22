#include "GameFont.h"

GameFont::GameFont() : m_font(0)
{
}

GameFont::GameFont(const GameFont &)
{
}

GameFont::~GameFont()
{
}

bool GameFont::Initialize(ID3D11Device * device, char * fontFilename, wchar_t * textureFilename)
{
	bool result;

	result = LoadFontData(fontFilename);
	if (!result) {
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if (!result) {
		return false;
	}

	return true;
}

void GameFont::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

ID3D11ShaderResourceView * GameFont::GetTexture()
{
	return nullptr;
}

void GameFont::BuildVertexArray(void *, char *, float, float)
{
}

bool GameFont::LoadFontData(char * filename)
{
	std::ifstream fin;
	int i;
	char temp;

	m_font = new FontType[95];
	if (!m_font) {
		return false;
	}

	fin.open(filename);
	if (fin.fail()) {
		return false;
	}

	for (i = 0; i < 95; i++) {
		fin.get(temp);
		while (temp != ' ')
		{

		}
	}
}

void GameFont::ReleaseFontData()
{
}
