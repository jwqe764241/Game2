#pragma once

#include <vector>
#include <Sources/GameApp.h>

#include <Sources/GameDefs.h>
#include <Sources/GameAssetLoader.h>
#include <Sources/Camera/GameCamera.h>
#include <Sources/Input/GameInput2.h>
#include <Sources/Interface/IRenderable.h>
#include <Sources/Interface/ILevel.h>

/*
	일단 리스트 2개이고, for문 2개지만 아마 어떻게 해결 할 방법이 있을 것이다 ㅋㅋ...
	맘에 안들긴 함

	렌더 우선순위를 어떻게 관리할 수 있나 생각중이긴 한데 (몇개 있긴 하다)
	근데 이렇게 기능들을 추가해버리면 과연 거기가서 칠 수 있으련지
*/

using EnvironmentList = std::vector<IRenderable *>;
using ActorList = std::vector<ICharacter *>;

class TestLevel1 : public ILevel{
private:
	/*
		풀 같은 것들
	*/
	EnvironmentList m_EnvironmentList;
	/*
		몬스터 같은 것들
	*/
	ActorList m_ActorList;
	Player* m_Player;
	CGameCamera m_Camera;

	const D3DXVECTOR2 m_PosList[10] = {
		{ 30, 50 },
		{ 60, 400 },
		{ 100, 50 },
		{ 300, 60 },
		{ 700, 500 },
		{ 550, 300 },
		{ 340, 400 },
		{ 200, 350 },
		{ 200, 500 },
		{ 30, 500 },
	};
	const D3DXVECTOR2 m_SpawnPoint[5] = {
		{200, 0},
		{700, 500},
		{500, 40},
		{300, 300},
		{700, 500}
	};

	RECT m_LevelSize;

public:
	TestLevel1();
	~TestLevel1();

	virtual bool Load() override;
	virtual void Unload() override;
	virtual void Update(float dt) override;
	virtual bool Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight) override;

	virtual void onStart() override;
	virtual void onEnd() override;
};