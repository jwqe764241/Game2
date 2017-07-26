//#ifndef __COLORSHADER_H
//#define __COLORSHDAER_H
//
//#include <Windows.h>
//#include <D3DX11.h>
//#include <D3D11.h>
//#include <atlbase.h>
//#include <xnamath.h>
//#include <D3DX10math.h>
//#include <D3DX11async.h>
//#include <fstream>
//
//#include <Sources/Utils/error.h>
//
//class ColorShader
//{
//private:
//	struct MatrixBufferType
//	{
//		D3DXMATRIX world;
//		D3DXMATRIX view;
//		D3DXMATRIX projection;
//	};
//
//public:
//	ColorShader();
//	~ColorShader();
//
//	bool Initialize(ID3D11Device*, HWND);
//	void Release();
//	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
//
//private:
//	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
//	void ShutdownShader();
//	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
//
//	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
//	void RenderShader(ID3D11DeviceContext*, int);
//
//private:
//	ID3D11VertexShader* m_vertexShader;
//	ID3D11PixelShader* m_pixelShader;
//	ID3D11InputLayout* m_inputLayout;
//	ID3D11Buffer* m_MatrixBuffer;
//};
//
//#endif
