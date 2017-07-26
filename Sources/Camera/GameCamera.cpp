#include <Sources/Camera/GameCamera.h>

CGameCamera::CGameCamera() :
	m_Position({0.0f, 0.0f, 0.0f}), m_Rotation({0.0f, 0.0f, 0.0f})
{

}

CGameCamera::~CGameCamera()
{

}

void CGameCamera::SetPosition(float x, float y, float z)
{
	m_Position = { x, y, z };
}

void CGameCamera::SetPosition(const D3DXVECTOR3& position)
{
	m_Position = position;
}

void CGameCamera::SetRotation(float x, float y, float z)
{
	m_Rotation = { x, y, z };
}

void CGameCamera::SetRotation(const D3DXVECTOR3& rotation)
{
	m_Rotation = rotation;
}

D3DXVECTOR3 CGameCamera::GetPosition() const
{
	return m_Position;
}

D3DXVECTOR3 CGameCamera::GetRotation() const
{
	return m_Rotation;
}

void CGameCamera::Render()
{
	D3DXVECTOR3 up     = {0.0f, 1.0f, 0.0f};
	D3DXVECTOR3 pos    = m_Position;
	D3DXVECTOR3 lookAt = {0.0f, 0.0f, 1.0f};

	float roll, pitch, yaw;
	D3DXMATRIX rotationMatrix;

	//¶óµð¾È
	roll  = m_Rotation.z * 0.0174532925f;
	pitch = m_Rotation.x * 0.0174532925f;
	yaw   = m_Rotation.y * 0.0174532925f;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	lookAt += pos;

	D3DXMatrixLookAtLH(&m_ViewMatrix, &pos, &lookAt, &up);
}

D3DXMATRIX& CGameCamera::GetViewMatrix()
{
	return m_ViewMatrix;
}
