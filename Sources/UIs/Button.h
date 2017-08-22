#pragma once

#include "Sources\Geometries\GameBitmap.h"
#include "Sources\Interface\IRenderable.h"

class Button : public IRenderable
{
public:
	Button(wchar_t* buttonText = L"Button", float x = 0, float y = 0, float width = 50, float height = 20);
	~Button();

	//비트맵, 버퍼 등, 렌더 가능한 것들에 대한 자원들 로드
	virtual void Load(ID3D11Device* device, int width, int height) override;
			void Load(ID3D11Device* device, wchar_t* normalImagePath, wchar_t* hoveredImagePath = nullptr, wchar_t* clickedImagePath = nullptr);
	//렌더 가능한 것들에서 사용된 자원 해제
	virtual void Release() override;
	//렌더 가능한 것들의 상태를 초기화
	virtual void Reset() override;
	//렌더 가능한 것들의 상태를 업데이트
	virtual void Update(float dt) override;
			void Update(float x, float y);

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

	virtual GameSprite* GetSprite() = 0;

private:
	bool IsHovered(float x, float y);


private:
	GameBitmap m_normalButtonImage,
			   m_hoveredButtonImage,
			   m_clickedButtonImage;

	wchar_t *m_normalButtonImagePath,
			*m_hoveredButtonImagePath,
			*m_clickedButtonImagePath;

	float m_posX, m_posY;

	float m_width, m_height;

	bool m_hovered, m_clicked;
};