#pragma once

#include <d3dx10math.h>

class CGameCamera 
{
private:
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Rotation;
	D3DXMATRIX ViewMatrix;

public:
	CGameCamera();
	~CGameCamera();

	//카메라 위치 설정 -> x, y, z
	void SetPosition(float x, float y, float z);
	void SetPosition(const D3DXVECTOR3& position);

	//카메라 회전 설정 -> 되도록 사용 하지 않기
	void SetRotation(float x, float y, float z);
	void SetRotation(const D3DXVECTOR3& rotation);

	//카메라 정보 가져오기
	D3DXVECTOR3 GetPosition() const;
	D3DXVECTOR3 GetRotation() const;

	//카메라 렌더 (행렬 변경)
	void Render();
	//뷰 행렬 가져오기
	D3DXMATRIX& GetViewMatrix();
};