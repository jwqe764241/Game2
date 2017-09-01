#include <Sources/Assets/Player.h>

static float waterCoolDown = 0.0f;
static float foodCoolDown = 0.0f;
static float sleepCoolDown = 0.0f;

template<typename T, typename U>
int GetPos(T& container, U& value_to_find)
{
	return std::distance(container.begin(), std::find_if(container.begin(), container.end(), [&value_to_find](const U* p) { return *p == value_to_find; }));
}

Player::Player() : Sprite(15.0f, 1.0f, true), ResourcePath(L"../Resources/sprite.bmp")
{

}

Player::~Player()
{

}

void Player::AddTool(Tool * pTool)
{
	ToolSink.push_back(pTool);
}

void Player::Load(ID3D10Device * device, int bitmapWidth, int bitmapHeight)
{
	ItemSink.reserve(10);

	Position.x = 0;
	Position.y = 0;
	Health = 100;
	Sprite.Initialize(device, ResourcePath, bitmapWidth, bitmapHeight, 4);
	Sprite.SetLooping(false);
	Sprite.SetMotion(2);
	IsSetPositionLimit = false;
	
	WaterValue = 100;
	FoodValue = 100;
	SleepValue = 100;

	for (int i = 0; i < g_PreDefinedItemAmount; ++i)
	{
		ItemSink.push_back(new Item(i, g_ItemResourcePathList[i].item_name));
		ItemSink[i]->Initialize(device, g_ItemResourcePathList[i].resource_path, 50, 50);
	}
}

void Player::Release()
{
	for (auto& target : ToolSink)
	{
		Utils::Release(&target);
	}

	Sprite.Release();
}

void Player::Reset()
{

}

void Player::Update(float dt)
{
	waterCoolDown += dt;
	foodCoolDown += dt;
	sleepCoolDown += dt;

	if (waterCoolDown >= 3.0f)
	{
		WaterValue -= 1;
		waterCoolDown = 0.0f;
	}
	if (foodCoolDown >= 4.0f) 
	{
		FoodValue -= 1;
		foodCoolDown = 0.0f;
	}
	if (sleepCoolDown >= 5.0f)
	{
		SleepValue -= 1;
		sleepCoolDown = 0.0f;
	}

	GameInput2& input = GameInput2::GetInstance();
	float x = 0.0f, y = 0.0f;
	float speed = 350.0f;

	if (input.IsPressed(VK_RIGHT))
	{
		x += 1.0f;
		Sprite.SetMotion(3);
	}
	else if (input.IsPressed(VK_LEFT))
	{
		x -= 1.0f;
		Sprite.SetMotion(1);
	}
	else if (input.IsPressed(VK_UP))
	{
		y -= 1.0f;
		Sprite.SetMotion(0);
	}
	else if (input.IsPressed(VK_DOWN))
	{
		y += 1.0f;
		Sprite.SetMotion(2);
	}
	else
	{
		Sprite.SetLooping(false);
		Sprite.Update(dt);
		return;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0) {
		speed *= dt;
		x *= speed / length;
		y *= speed / length;
		if (IsSetPositionLimit)
		{
			if (Position.x + x >= PositionLimit.left && (Position.x + x) + Sprite.GetFrameWidth() <= PositionLimit.right)
			{
				Position.x += x;
			}

			if (Position.y + y >= PositionLimit.top && (Position.y + y) + Sprite.GetFrameHeight() <= PositionLimit.bottom)
			{
				Position.y += y;
			}
		}
		else
		{
			Position.x += x;
			Position.y += y;
		}
	}

	Sprite.SetLooping(true);
	Sprite.Update(dt);
}

void Player::Update(float dt, CGameCamera* pCamera)
//해당 업데이트는 마우스 클릭 시 해당 마우스 좌표로 이동하게 하는 시뮬레이션 함수임
{
	static bool isWalking = false;
	static POINT targetPoint{ 0.0f, 0.0f };

	float x = 0.0f, y = 0.0f;
	float speed = 350.0f;

	if (GameInput2::GetInstance().IsPressed(VK_LBUTTON))
	{
		POINT mousePos = GameInput2::GetInstance().GetMousePosition();
		D3DXVECTOR3 cameraPos = pCamera->GetPosition();
		//Move(D3DXVECTOR2{ po.x + cameraPos.x, po.y - cameraPos.y});
		targetPoint.x = mousePos.x + cameraPos.x - 32;
		targetPoint.y = mousePos.y - cameraPos.y - 32;
		isWalking = true;
	}
	
	if (!isWalking)
	{
		return;
	}

	if (Position.x < targetPoint.x)
	{
		x += 1.0f;
	}
	else
	{
		x -= 1.0f;
	}

	if (Position.y < targetPoint.y)
	{
		y += 1.0f;
	}
	else
	{
		y -= 1.0f;
	}

	float length = sqrt(pow(x, 2) + pow(y, 2));

	if (length > 0) {

		if ((Position.x >= (targetPoint.x - 1.0f)) && (Position.x <= (targetPoint.x + 1.0f)))
		{
			if ((Position.y >= (targetPoint.y - 1.0f)) && (Position.y <= (targetPoint.y + 1.0f)))
			{
				isWalking = false;
				Sprite.SetLooping(false);
				return;
			}
		}

		speed *= dt;
		x *= speed / length;
		y *= speed / length;

		if (IsSetPositionLimit)
		{
			if (Position.x >= PositionLimit.left && Position.x + Sprite.GetFrameWidth() / 2 <= PositionLimit.right)
			{
				Position.x += x;
			}

			if (Position.y >= PositionLimit.top && Position.y + Sprite.GetFrameHeight() / 2 <= PositionLimit.bottom)
			{
				Position.y += y;
			}
		}
		else
		{
			Position.x += x;
			Position.y += y;
		}

	}

	Sprite.SetLooping(true);
	Sprite.Update(dt);
}

void Player::Render(ID3D10Device * device, int screenWidth, int screenHeight)
{
	Sprite.Render(device, screenWidth, screenHeight, Position.x, Position.y);
}

void Player::Idle()
{
	Sprite.SetLooping(false);
}

//어디다가 쓰지..
void Player::Move(D3DXVECTOR2 target)
{
	Position.x = target.x;
	Position.y = target.y;
}

void Player::Attack(void ** target)
{

}

void Player::Damage(int damage)
{
	if (Health - damage <= 0)
	{
		Health = 0;
	}
	else
	{
		Health -= damage;
	}
}

void Player::Die()
{
	Health = 0;
}

bool Player::isDied()
{
	if (Health <= 0)
	{
		return false;
	}
	return false;
}

int Player::GetIndexCount()
{
	return Sprite.GetIndexCount();
}

ID3D10ShaderResourceView* Player::GetTexture()
{
	return Sprite.GetTexture();
}

D3DXVECTOR2 Player::GetPosition() const
{
	return Position;
}

void Player::SetPosition(const D3DXVECTOR2 pos)
{
	Position = pos;
}

std::vector<Tool *>& Player::GetToolList()
{
	return ToolSink;
}

std::vector<Item *>& Player::GetItemList()
{
	return ItemSink;
}

void Player::AddItem(int itemID, int amount)
{
	//함수로 묶기

	Item item(itemID, "Dummy");

	int pos = GetPos(ItemSink, item);

	ItemSink[pos]->AddAmount(amount);
}

bool Player::SubItem(int itemID, int amount)
{
	//함수로 묶기

	Item item(itemID, "Dummy");

	int pos = GetPos(ItemSink, item);

	return ItemSink[pos]->SubAmount(amount);
}

int Player::GetItemAmount(int itemID)
{
	//함수로 묶기

	Item item(itemID, "Dummy");

	int pos = GetPos(ItemSink, item);

	return ItemSink[pos]->GetAmount();
}

GameSprite* Player::GetSprite()
{
	return &Sprite;
}

void Player::SetPositionLimit(const RECT* limitPos)
{
	if (limitPos == nullptr)
	{
		IsSetPositionLimit = false;
		PositionLimit = { 0 };
	}
	else
	{
		IsSetPositionLimit = true;
		PositionLimit = (*limitPos);
	}
}

int Player::GetHealth() const
{
	return Health;
}

int Player::GetWaterValue() const
{
	return WaterValue;
}

int Player::GetFoodValue() const
{
	return FoodValue;
}

int Player::GetSleepValue() const
{
	return SleepValue;
}

void Player::SetWaterValue(int value)
{
	if (value >= 100)
	{
		WaterValue = 100;
	}
	else
	{
		WaterValue = value;
	}
}

void Player::SetFoodValue(int value)
{
	if (value >= 100)
	{
		FoodValue = 100;
	}
	else
	{
		FoodValue = value;
	}
}

void Player::SetSleepValue(int value)
{
	if (value >= 100)
	{
		SleepValue = 100;
	}
	else
	{
		SleepValue = value;
	}
}