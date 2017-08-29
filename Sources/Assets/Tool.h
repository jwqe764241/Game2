#pragma once

#include <d3d10_1.h>
#include <Sources/Geometries/GameBitmap.h>

struct ToolResourceElement
{
	wchar_t* tool_name;
	wchar_t* resource_path;
};

static constexpr int g_PreDefinedToolAmount = 5;

static const ToolResourceElement g_ToolList[g_PreDefinedToolAmount]{
	{ L"saw", L"../Resources/tools/saw.png" },
	{ L"pickaxe", L"../Resources/tools/pickaxe.png" },
	{ L"hammer",  L"../Resources/tools/hammer.png" },
	{ L"bottle", L"../Resources/tools/bottle.png" },
	{ L"axe", L"../Resources/tools/axe.png" }
};

class Tool : public GameBitmap {
private:
	wchar_t* m_ToolName;
	int m_ToolID;
	POINT m_Pos;

private:
	bool UpdateBuffers(ID3D10Device *device, int screenWidth, int screenHeight, int posX, int posY);
	void RenderBuffers(ID3D10Device *device);

public:
	Tool() = delete;
	Tool(wchar_t* toolName, int toolID);
	~Tool();

	void Initialize(ID3D10Device* device, wchar_t* filePath, int bitmapWidth, int bitmapHeight, POINT pos);
	bool Render(ID3D10Device* device, int screenWidth, int screenHeight);
	bool Render(ID3D10Device* device, int screenWidth, int screenHeight, int posX, int posY);
	void Release();

	ID3D10ShaderResourceView* GetTexture();
	int GetIndexCount();
	wchar_t* GetToolName() const;
	int GetToolID() const;
	POINT GetPosition() const;

	friend bool operator==(Tool& lhs, Tool& rhs);
};