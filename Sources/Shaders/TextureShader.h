#ifndef __TEXTURESHADER_H
#define __TEXTURESHADER_H

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
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
	bool Initialize(ID3D11Device *device, HWND hwnd);
	void Release();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView *texture);

private:
	bool InitializeShader(ID3D11Device *device, HWND hwnd, wchar_t *vertexShaderPath, wchar_t *pixelShaderPath);
	void ReleaseShader();
	
	void HandlingError(ID3D10Blob *error, HWND hwnd, wchar_t *outFileName, wchar_t *title, wchar_t *caption);
	
	bool SetParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView *texture);
	void RenderShader(ID3D11DeviceContext *deviceContext, int);

private:
	ID3D11VertexShader *m_VertexShader;
	ID3D11PixelShader *m_PixelShader;
	ID3D11InputLayout *m_InputLayout;
	ID3D11Buffer *m_MatrixBuffer;
	ID3D11SamplerState *m_SampleState;
};

#endif