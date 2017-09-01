#pragma once

#include <cstring>
#include <string>
#include <Sources/Geometries/GameBitmap.h>

//아이템 이름 리스트

struct ItemResourceElement
{
	std::string item_name;
	wchar_t * resource_path;
};

static constexpr int g_PreDefinedItemAmount = 8;

static const ItemResourceElement g_ItemResourcePathList[g_PreDefinedItemAmount]
{
	{ "Citrus", L"../Resources/items/cirtus.png" },
	{ "Leaf", L"../Resources/items/leaf.png" },
	{ "Stem", L"../Resources/items/stem.png" },
	{ "Firewood", L"../Resources/items/firewood.png" },
	{ "Meat", L"../Resources/items/meat.png" },
	{ "Leather", L"../Resources/items/leather.png" },
	{ "Pebble", L"../Resources/items/pebble.png" },
	{ "Rope", L"../Resources/items/rope.png" }
};

class Item : public GameBitmap{
private:
	int Amount;

	int ItemID;
	std::string ItemName;
public:
	Item() = delete;
	Item(int itemID, std::string itemName);
	~Item();

	bool Initialize(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight);
	bool Render(ID3D10Device* device, int screenWidth, int screenHeight, int posX, int posY);
	void RenderBuffers(ID3D10Device * device);
	bool UpdateBuffers(ID3D10Device *device, int screenWidth, int screenHeight, int posX, int posY);
	void Release();

	void AddAmount(int amount);
	bool SubAmount(int amount);
	int  GetAmount();
	ID3D10ShaderResourceView* GetTexture();

	int GetID() const;
	std::string GetItemName() const;

	friend bool operator== (const Item& lhs, const Item& rhs);
};