#include "TextureShader.h"

TextureShader::TextureShader() :
	m_effect(nullptr), m_technique(nullptr), m_layout(nullptr),
	m_worldMatrixPtr(nullptr), m_viewMatrixPtr(nullptr), m_projectionMatrixPtr(nullptr),
	m_texturePtr(nullptr)
{
}

TextureShader::~TextureShader()
{	
	Release();
}

TextureShader& TextureShader::GetInstance()
{
	static TextureShader instance;
	
	return instance;
}

bool TextureShader::Initialize(ID3D10Device *device, HWND hwnd)
{
	if (!InitializeShader(device, hwnd, L"..\\Sources\\Shaders\\texture.fx") )
	{
		return false;
	}

	return true;
}

void TextureShader::Release()
{
	ReleaseShader();
}

bool TextureShader::Render(ID3D10Device* device, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView *texture)
{
	if (!SetParameters(worldMatrix, viewMatrix, projectionMatrix, texture)) 
	{
		return false;
	}

	RenderShader(device, indexCount);
	return true;
}

bool TextureShader::InitializeShader(ID3D10Device* device, HWND hwnd, wchar_t *filePath)
{
	HRESULT result;
	ID3D10Blob *error = nullptr;

	result = D3DX10CreateEffectFromFile(filePath, NULL, NULL, "fx_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, 
		device, NULL, NULL, &m_effect, &error, NULL);
	if (FAILED(result)) 
	{
		HandlingError(error, hwnd, L"shader_error_out.txt", filePath, L"check shader_error_out.txt!");
		return false;
	}

	m_technique = m_effect->GetTechniqueByName("TextureTechnique");
	if (!m_technique)
	{
		return false;
	}

	D3D10_INPUT_ELEMENT_DESC polyLayout[2];
		polyLayout[0].SemanticName         = "POSITION";
		polyLayout[0].SemanticIndex        = 0;
		polyLayout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		polyLayout[0].InputSlot            = 0;
		polyLayout[0].AlignedByteOffset    = 0;
		polyLayout[0].InputSlotClass       = D3D10_INPUT_PER_VERTEX_DATA;
		polyLayout[0].InstanceDataStepRate = 0;

		polyLayout[1].SemanticName         = "TEXCOORD";
		polyLayout[1].SemanticIndex        = 0;
		polyLayout[1].Format               = DXGI_FORMAT_R32G32_FLOAT;
		polyLayout[1].InputSlot            = 0;
		polyLayout[1].AlignedByteOffset    = D3D10_APPEND_ALIGNED_ELEMENT;
		polyLayout[1].InputSlotClass       = D3D10_INPUT_PER_VERTEX_DATA;
		polyLayout[1].InstanceDataStepRate = 0;

	int numElements = sizeof(polyLayout) / sizeof(polyLayout[0]);

	D3D10_PASS_DESC passDesc;
	m_technique->GetPassByIndex(0)->GetDesc(&passDesc);

	result = device->CreateInputLayout(polyLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_layout);
	if (FAILED(result)) 
	{
		MessageBox(hwnd, L"Cannot Create InputLayout", L"Error", MB_OK);
		return false;
	}

	m_worldMatrixPtr = m_effect->GetVariableByName("worldMatrix")->AsMatrix();
	m_viewMatrixPtr = m_effect->GetVariableByName("viewMatrix")->AsMatrix();
	m_projectionMatrixPtr = m_effect->GetVariableByName("projectionMatrix")->AsMatrix();

	m_texturePtr = m_effect->GetVariableByName("shaderTexture")->AsShaderResource();

	/*D3D10_BUFFER_DESC matrixBufferDesc;
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
	}*/

	return true;
}

void TextureShader::ReleaseShader()
{
	Utils::Release(&m_effect);
	Utils::Release(&m_layout);

	m_technique           = 0;
	m_worldMatrixPtr      = 0;
	m_viewMatrixPtr       = 0;
	m_projectionMatrixPtr = 0;
	m_texturePtr          = 0;
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

bool TextureShader::SetParameters(D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D10ShaderResourceView *texture)
{
	/*HRESULT result;
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
	deviceContext->PSSetShaderResources(0, 1, &texture);*/

	m_worldMatrixPtr->SetMatrix(reinterpret_cast<float *>(&worldMatrix));
	m_viewMatrixPtr->SetMatrix(reinterpret_cast<float *>(&viewMatrix));
	m_projectionMatrixPtr->SetMatrix(reinterpret_cast<float *>(&projectionMatrix));

	m_texturePtr->SetResource(texture);

	return true;
}

void TextureShader::RenderShader(ID3D10Device* device, int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;

	device->IASetInputLayout(m_layout);

	m_technique->GetDesc(&techniqueDesc);

	for (int i = 0; i<techniqueDesc.Passes; ++i)
	{
		m_technique->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}

}
