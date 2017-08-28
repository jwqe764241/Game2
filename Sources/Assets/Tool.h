#pragma once

#include <d3d10_1.h>
#include <Sources/Geometries/GameBitmap.h>

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
};