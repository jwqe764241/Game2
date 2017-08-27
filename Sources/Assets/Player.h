#pragma once

#include <D3DX11.h>
#include <vector>
#include <Sources/GameDefs.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Geometries/GameSprite.h>
#include <Sources/Interface/ICharacter.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Assets/Tool.h>

class Player : public ICharacter
{
private:
	wchar_t* m_ResourcePath;

	int m_Health;
	int m_WaterValue;
	int m_FoodValue;
	int m_SleepValue;
	D3DXVECTOR2 m_Pos;
	GameSprite m_Sprite;

	bool isSetPositionLimit;
	RECT m_PositionLimit;

	std::vector<Tool *> m_ToolSink;

public:
	Player();
	~Player();

	void AddTool(Tool * pTool);

	virtual void Load(ID3D11Device* device, int bitmapWidth, int bitmapHeight) override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
			void Update(float dt, CGameCamera* pCamera);
	virtual void Render(ID3D11DeviceContext*, int screenWidth, int screenHeight) override;

	virtual void Idle() override;
	virtual void Move(D3DXVECTOR2 target) override;
	virtual void Attack(void** target) override; // attack close target or target attack.
	//virtual void Attack(XMVECTORF32 direction, Wepon wepon); // long target or non-target attack.
	virtual void Damage(int damage) override;
	virtual void Die() override;
	virtual bool isDied() override;

	virtual int GetIndexCount() override;
	virtual ID3D11ShaderResourceView* GetTexture() override;

	virtual D3DXVECTOR2 GetPosition() const override;
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
	virtual GameSprite* GetSprite() override;
	
	/*
		NULL일 경우에 설정 안함, 값이 있으면 설정 함
	*/
	void SetPositionLimit(const RECT* limitPos);
	std::vector<Tool *>& GetToolList();

	int GetHealth() const;
	int GetWaterValue() const;
	int GetFoodValue() const;
	int GetSleepValue() const;
};