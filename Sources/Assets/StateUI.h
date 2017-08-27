#pragma once

#include <vector>
#include <Sources/Assets/Tool.h>
#include <Sources/Geometries/StateBar.h>
#include <Sources/Shaders/TextureShader.h>

class StateUI {
private:
	StateBar stateBar[5];
	std::vector<Tool *>* m_ToolList;

public: 
	StateUI();
	~StateUI();

	void Initialize(std::vector<Tool *>* toolList);
	void Release();
	void Update(int healthValue, int waterValue, int foodValue, int sleepValue);
	void Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight,
		D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthMatrix, D3DXVECTOR3 cameraPos, TextureShader& instance);
};