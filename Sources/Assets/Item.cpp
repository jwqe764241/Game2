#include <Sources/Assets/Item.h>

bool operator== (const Item& lhs, const Item& rhs)
{
	return lhs.m_ItemName == rhs.m_ItemName;
}

Item::Item(int itemID, std::string itemName)
{
	m_ID = itemID;
	m_ItemName = itemName;
}

Item::~Item()
{

}

bool Item::Initialize(ID3D10Device * device, wchar_t * filePath, int bitmapWidth, int bitmapHeight)
{
	if (!m_ItemBitmap.Initialize(device, filePath, bitmapWidth, bitmapHeight))
	{
		return false;
	}
	
	m_Amount = 0;

	return true;
}

void Item::Release()
{
	m_ItemBitmap.Release();
}

void Item::AddAmount(int amount)
{
	m_Amount += amount;
}

bool Item::SubAmount(int amount)
{
	if ((m_Amount - amount) < 0)
	{
		return false;
	}
	else
	{
		m_Amount -= amount;
		return true;
	}
}

int Item::GetAmount()
{
	return m_Amount;
}

ID3D10ShaderResourceView* Item::GetTexture()
{
	return m_ItemBitmap.GetTexture();
}