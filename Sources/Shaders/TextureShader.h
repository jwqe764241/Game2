#pragma once

#include <d3d10_1.h>
#include <d3dx10math.h>
#include <fstream>

#include <Sources/GameDefs.h>

class TextureShader
{
private:
	struct _Matrix {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	using Matrix = _Matrix;

private:
	ID3D10Effect* Effect;
	ID3D10EffectTechnique* EffectTechnique;
	ID3D10InputLayout* InputLayout;

	ID3D10EffectMatrixVariable* WorldMatrixPtr;
	ID3D10EffectMatrixVariable* ViewMatrixPtr;
	ID3D10EffectMatrixVariable* ProjectionMatrixPtr;
	ID3D10EffectShaderResourceVariable* TexturePtr;

private:
	bool InitializeShader(ID3D10Device* device, HWND hwnd, wchar_t *filePath);
	void ReleaseShader();

	void HandlingError(ID3D10Blob* error, HWND hwnd, wchar_t* outFileName, wchar_t* title, wchar_t* caption);

	bool SetParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView *texture);
	void RenderShader(ID3D10Device* device, int indexCount);

public:
	TextureShader();
	virtual ~TextureShader();

	static TextureShader& GetInstance();
	bool Initialize(ID3D10Device *device, HWND hwnd);
	void Release();
	bool Render(ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView *texture);
};