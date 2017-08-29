#pragma once

#include <vector>
#include <Sources/GameTextWriter.h>
#include <Sources/Assets/Tool.h>
#include <Sources/Assets/Item.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Geometries/StateBar.h>
#include <Sources/Shaders/TextureShader.h>

class StateUI {
private:
	StateBar stateBar[5];
	std::vector<Tool *>* m_ToolList;
	std::vector<Item *>* m_ItemList;

	GameTextWriter writer;

public: 
	StateUI();
	~StateUI();

	void Initialize(ID3D10Device * pDevice, std::vector<Tool *>* pToolList, std::vector<Item *>* pItemList);
	void Release();
	void Update(int healthValue, int waterValue, int foodValue, int sleepValue);
	void Render(ID3D10Device* device, int screenWidth, int screenHeight,
		D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthMatrix, D3DXVECTOR3 cameraPos, TextureShader& instance);
};