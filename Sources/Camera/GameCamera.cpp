#include <Sources/Camera/GameCamera.h>

CGameCamera::CGameCamera() :
	Position({0.0f, 0.0f, 0.0f}), Rotation({0.0f, 0.0f, 0.0f})
{

}

CGameCamera::~CGameCamera()
{

}

void CGameCamera::SetPosition(float x, float y, float z)
{
	Position = { x, y, z };
}

void CGameCamera::SetPosition(const D3DXVECTOR3& position)
{
	Position = position;
}

void CGameCamera::SetRotation(float x, float y, float z)
{
	Rotation = { x, y, z };
}

void CGameCamera::SetRotation(const D3DXVECTOR3& rotation)
{
	Rotation = rotation;
}

D3DXVECTOR3 CGameCamera::GetPosition() const
{
	return Position;
}

D3DXVECTOR3 CGameCamera::GetRotation() const
{
	return Rotation;
}

void CGameCamera::Render()
{
	D3DXVECTOR3 up     = {0.0f, 1.0f, 0.0f}; //--> 카메라는 고정이기 때문에 해당 값으로 초기화
	D3DXVECTOR3 pos    = Position;
	D3DXVECTOR3 lookAt = {0.0f, 0.0f, 1.0f};

	float roll, pitch, yaw;
	D3DXMATRIX rotationMatrix;

	//라디안으로 변환
	//D3DXMatrixRotationYawPitchRoll()가 라디안 단위이기 때문에
	roll  = Rotation.z * 0.0174532925f;
	pitch = Rotation.x * 0.0174532925f;
	yaw   = Rotation.y * 0.0174532925f;

	//roll, pitch, yaw를 이용하여 ratationMatrix 생성
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt += pos;

	//뷰 행렬 생성
	D3DXMatrixLookAtLH(&ViewMatrix, &pos, &lookAt, &up);
}

D3DXMATRIX& CGameCamera::GetViewMatrix()
{
	return ViewMatrix;
}
