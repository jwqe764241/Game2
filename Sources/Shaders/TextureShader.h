#pragma once

#include <d3d10_1.h>
#include <d3dx10math.h>
#include <fstream>

#include <Sources/GameDefs.h>

class TextureShader {
private:
	struct _Matrix {
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	using Matrix = _Matrix;

public:
	TextureShader();
	~TextureShader();

	static TextureShader& GetInstance();
	bool Initialize(ID3D10Device *device, HWND hwnd);
	void Release();
	bool Render(ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView *texture);

private:
	bool InitializeShader(ID3D10Device* device, HWND hwnd, wchar_t *filePath);
	void ReleaseShader();
	
	void HandlingError(ID3D10Blob *error, HWND hwnd, wchar_t *outFileName, wchar_t *title, wchar_t *caption);
	
	bool SetParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView *texture);
	void RenderShader(ID3D10Device* device, int indexCount);

private:
	ID3D10Effect* m_effect;
	ID3D10EffectTechnique* m_technique;
	ID3D10InputLayout* m_layout;

	ID3D10EffectMatrixVariable* m_worldMatrixPtr;
	ID3D10EffectMatrixVariable* m_viewMatrixPtr;
	ID3D10EffectMatrixVariable* m_projectionMatrixPtr;
	ID3D10EffectShaderResourceVariable* m_texturePtr;
};