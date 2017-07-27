#pragma once

#include <D3D11.h>

class IRenderable
{
public:
	virtual void Load(ID3D11Device*, int, int, int, int) = 0;
	virtual void Release()        = 0;
	virtual void Reset()          = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(ID3D11DeviceContext*, int, int) = 0;
	virtual int GetIndexCount() = 0;
	virtual ID3D11ShaderResourceView* GetTexture() = 0;
};