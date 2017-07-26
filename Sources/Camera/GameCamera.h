#ifndef __GAMECAMERA_H
#define __GAMECAMERA_H

#include <D3DX10math.h>

class CGameCamera {
public:
	CGameCamera();
	~CGameCamera();

	void SetPosition(float x, float y, float z);
	void SetPosition(const D3DXVECTOR3& position);

	void SetRotation(float x, float y, float z);
	void SetRotation(const D3DXVECTOR3& rotation);

	D3DXVECTOR3 GetPosition() const;
	D3DXVECTOR3 GetRotation() const;

	void Render();
	D3DXMATRIX& GetViewMatrix();

private:
	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation;
	D3DXMATRIX m_ViewMatrix;	
};

#endif