#pragma once

#include <vector>
#include <Sources/GameTextWriter.h>
#include <Sources/Assets/Tool.h>
#include <Sources/Assets/Item.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Geometries/StateBar.h>
#include <Sources/Shaders/TextureShader.h>

static constexpr int ItemRenderBasePosX = 1725;
static constexpr int ItemRenderBasePosXOffset = 80;
static constexpr int ItemRenderBasePosY = 400;
static constexpr int ItemRenderBasePosYOffset = 60;

static const POINT ItemRenderPosition[g_PreDefinedItemAmount]
{
	{ ItemRenderBasePosX, ItemRenderBasePosY },		                            { ItemRenderBasePosX + ItemRenderBasePosXOffset, ItemRenderBasePosY },
	{ ItemRenderBasePosX, ItemRenderBasePosY + ItemRenderBasePosYOffset },      { ItemRenderBasePosX + ItemRenderBasePosXOffset, ItemRenderBasePosY + ItemRenderBasePosYOffset },
	{ ItemRenderBasePosX, ItemRenderBasePosY + (ItemRenderBasePosYOffset * 2)}, { ItemRenderBasePosX + ItemRenderBasePosXOffset, (ItemRenderBasePosY + ItemRenderBasePosYOffset * 2)},
	{ ItemRenderBasePosX, ItemRenderBasePosY + (ItemRenderBasePosYOffset * 3)}, { ItemRenderBasePosX + ItemRenderBasePosXOffset, ItemRenderBasePosY + (ItemRenderBasePosYOffset * 3)}
};

class StateUI {
private:
	StateBar UIStateBar[5];
	std::vector<Tool *>* ToolList;
	std::vector<Item *>* ItemList;

	GameTextWriter writer_48size;
	GameTextWriter writer_25size;

public: 
	StateUI();
	~StateUI();

	void Initialize(ID3D10Device * pDevice, std::vector<Tool *>* pToolList, std::vector<Item *>* pItemList);
	void Release();
	void Update(int healthValue, int waterValue, int foodValue, int sleepValue);
	void Render(ID3D10Device* device, int screenWidth, int screenHeight,
		D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthMatrix, D3DXVECTOR3 cameraPos, TextureShader& instance);
};