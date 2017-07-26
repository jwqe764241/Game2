#include "TextureShader.h"

TextureShader::TextureShader() : m_VertexShader(nullptr), m_PixelShader(nullptr), m_InputLayout(nullptr), m_MatrixBuffer(nullptr), m_SampleState(nullptr)
{

}

TextureShader::~TextureShader()
{
	
}

bool TextureShader::Initialize(ID3D11Device *device, HWND hwnd)
{
	if (!InitializeShader(device, hwnd, L"..\\Sources\\Shaders\\texture_vs.vs", L"..\\Sources\\Shaders\\texture_ps.ps")) 
	{
		return false;
	}

	return true;
}

void TextureShader::Release()
{
	ReleaseShader();
}

bool TextureShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView *texture)
{

	if (!SetParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture)) 
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);
	return true;
}

bool TextureShader::InitializeShader(ID3D11Device * device, HWND hwnd, wchar_t * vertexShaderPath, wchar_t * pixelShaderPath)
{
	HRESULT result;
	ID3D10Blob *error = nullptr, *vsBuffer = nullptr, *psBuffer = nullptr;

	result = D3DX11CompileFromFile(vertexShaderPath, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vsBuffer, &error, NULL);
	if (FAILED(result)) 
	{
		HandlingError(error, hwnd, L"vertex_shader_error_out.txt", vertexShaderPath, L"check vertex_shader_error_out.txt!");
		return false;
	}

	result = D3DX11CompileFromFile(pixelShaderPath , NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &psBuffer, &error, NULL);
	if (FAILED(result)) 
	{
		HandlingError(error, hwnd, L"pixel_shader_error_out.txt", pixelShaderPath, L"check pixel_shader_error_out.txt!");
		return false;
	}

	result = device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &m_VertexShader);
	if (FAILED(result)) 
	{
		MessageBox(hwnd, L"Cannot Create VertexShader", L"Error", MB_OK);
		return false;
	}

	result = device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &m_PixelShader);
	if (FAILED(result)) 
	{
		MessageBox(hwnd, L"Cannot Create PixelShader", L"Error", MB_OK);
		return false;
	}


	D3D11_INPUT_ELEMENT_DESC polyLayout[2];
		polyLayout[0].SemanticName = "POSITION";
		polyLayout[0].SemanticIndex = 0;
		polyLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		polyLayout[0].InputSlot = 0;
		polyLayout[0].AlignedByteOffset = 0;
		polyLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polyLayout[0].InstanceDataStepRate = 0;

		polyLayout[1].SemanticName = "TEXCOORD";
		polyLayout[1].SemanticIndex = 0;
		polyLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		polyLayout[1].InputSlot = 0;
		polyLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		polyLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		polyLayout[1].InstanceDataStepRate = 0;

	int numElements = sizeof(polyLayout) / sizeof(polyLayout[0]);

	result = device->CreateInputLayout(polyLayout, numElements, vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &m_InputLayout);
	if (FAILED(result)) 
	{
		MessageBox(hwnd, L"Cannot Create InputLayout", L"Error", MB_OK);
		return false;
	}

	Utils::Release(&vsBuffer);
	Utils::Release(&psBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(Matrix);
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
		matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_MatrixBuffer);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Cannot Create Matrix Buffer", L"Error", MB_OK);
		return false;
	}

	D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = device->CreateSamplerState(&samplerDesc, &m_SampleState);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Cannot Create Sampler State", L"Error", MB_OK);
		return false;
	}

	return true;
}

void TextureShader::ReleaseShader()
{
	Utils::Release(&m_SampleState);
	Utils::Release(&m_MatrixBuffer);
	Utils::Release(&m_InputLayout);
	Utils::Release(&m_PixelShader);
	Utils::Release(&m_VertexShader);
}

void TextureShader::HandlingError(ID3D10Blob *error, HWND hwnd, wchar_t *outFileName, wchar_t *text, wchar_t *caption)
{
	if (error) 
	{
		char* errorMessage = reinterpret_cast<char *>(error->GetBufferPointer());
		unsigned long bufferSize = error->GetBufferSize();

		std::ofstream out(outFileName);
			out.write(errorMessage, bufferSize);
		out.close();

		Utils::Release(&error);

		MessageBox(hwnd, caption, text, MB_OK);
	}
	else 
	{
		MessageBox(hwnd, L"Shader File Not Found", text, MB_OK);
	}
}

bool TextureShader::SetParameters(ID3D11DeviceContext * deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView *texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	result = deviceContext->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	Matrix* pData = reinterpret_cast<Matrix*>(mappedResource.pData);
	pData->world = worldMatrix;
	pData->view = viewMatrix;
	pData->projection = projectionMatrix;

	deviceContext->Unmap(m_MatrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_MatrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_InputLayout);

	deviceContext->VSSetShader(m_VertexShader, NULL, 0);

	deviceContext->PSSetShader(m_PixelShader, NULL, 0);

	deviceContext->PSSetSamplers(0, 1, &m_SampleState);

	deviceContext->DrawIndexed(indexCount, 0, 0);
}
