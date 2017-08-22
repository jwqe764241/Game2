#pragma once

#include "Sources\Interface\IRenderable.h"

class TextView : public IRenderable
{
public:
	TextView();
	~TextView();

	//비트맵, 버퍼 등, 렌더 가능한 것들에 대한 자원들 로드
	virtual void Load(ID3D11Device* device, int bitmapWidth, int bitmapHeight) override;
	//렌더 가능한 것들에서 사용된 자원 해제
	virtual void Release() override;
	//렌더 가능한 것들의 상태를 초기화
	virtual void Reset() override;
	//렌더 가능한 것들의 상태를 업데이트
	virtual void Update(float dt) override;

	//렌더 1. 상속 받은 클래스에 좌표를 가지고 있을 경우에
	//virtual void Render(ID3D11DeviceContext* deviceContext) override;
	virtual void Render(ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight) override;

	//인덱스 갯수 가져오기
	virtual int GetIndexCount() override;
	//생성된 텍스쳐 가져오기
	virtual ID3D11ShaderResourceView* GetTexture() override;
	//포지션 가져오기
	virtual D3DXVECTOR2 GetPosition() const override;
	//포지션 설정하기
	virtual void SetPosition(const D3DXVECTOR2 pos) override;
	/*
	Renderable은 기본적으로 GameSprite를
	멤버변수로 가지고 있을 것이고,
	GameSprite의 값을 이용하여 타 에셋을 조정할 수
	있으므로 해당 함수를 추가하게 됨
	*/
	virtual GameSprite* GetSprite() override;

private:
	wchar_t *m_text;
};