#pragma once

#include <d3d10_1.h>
#include <vector>
#include <algorithm>
#include <Sources/GameDefs.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Geometries/GameSprite.h>
#include <Sources/Interface/ICharacter.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Assets/Tool.h>
#include <Sources/Assets/Item.h>

class Player : public ICharacter
{
private:
	wchar_t* ResourcePath;

	GameSprite Sprite;
	D3DXVECTOR2 Position;

	bool IsSetPositionLimit;
	RECT PositionLimit;

	std::vector<Tool *> ToolSink;
	std::vector<Item *> ItemSink;

	int Health;
	int WaterValue;
	int FoodValue;
	int SleepValue;

public:
	Player();
	~Player();

	void AddTool(Tool * pTool);

	virtual void Load(ID3D10Device* device, int bitmapWidth, int bitmapHeight) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
			void Update(float dt, CGameCamera* pCamera);
	virtual void Render(ID3D10Device*, int screenWidth, int screenHeight) override;

	virtual void Idle() override;
	virtual void Move(D3DXVECTOR2 target) override;
	virtual void Attack(void** target) override; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) override;
	virtual void Die() override;
	virtual bool isDied() override;

	virtual int GetIndexCount() override;
	virtual ID3D10ShaderResourceView* GetTexture() override;

	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
	virtual GameSprite* GetSprite() override;
	
	/*
		NULL일 경우에 설정 안함, 값이 있으면 설정 함
	*/
	void SetPositionLimit(const RECT* limitPos);
	std::vector<Tool *>& GetToolList();
	std::vector<Item *>& GetItemList();

	void AddItem(int itemID, int amount);
	bool SubItem(int itemID, int amount);
	int  GetItemAmount(int itemID);

	int GetHealth() const;
	int GetWaterValue() const;
	int GetFoodValue() const;
	int GetSleepValue() const;

	void SetWaterValue(int value);
	void SetFoodValue(int value);
	void SetSleepValue(int value);
};