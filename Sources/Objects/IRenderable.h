#pragma once

#include <D3D11.h>

class IRenderable
{
public:
	//비트맵, 버퍼 등, 렌더 가능한 것들에 대한 자원들 로드
	virtual void Load(ID3D11Device*, int, int, int, int) = 0;
	//렌더 가능한 것들에서 사용된 자원 해제
	virtual void Release()        = 0;
	//렌더 가능한 것들의 상태를 초기화
	virtual void Reset()          = 0;
	//렌더 가능한 것들의 상태를 업데이트
	virtual void Update(float dt) = 0;
	//출력
	virtual void Render(ID3D11DeviceContext*, int, int) = 0;
	
	//인덱스 갯수 가져오기
	virtual int GetIndexCount() = 0;
	//생성된 텍스쳐 가져오기
	virtual ID3D11ShaderResourceView* GetTexture() = 0;
};