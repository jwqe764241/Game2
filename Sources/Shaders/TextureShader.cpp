#include "TextureShader.h"

TextureShader::TextureShader() :
	Effect(nullptr), EffectTechnique(nullptr), InputLayout(nullptr),
	WorldMatrixPtr(nullptr), ViewMatrixPtr(nullptr), ProjectionMatrixPtr(nullptr),
	TexturePtr(nullptr)
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
		device, NULL, NULL, &Effect, &error, NULL);
	if (FAILED(result)) 
	{
		HandlingError(error, hwnd, L"shader_error_out.txt", filePath, L"check shader_error_out.txt!");
		return false;
	}

	EffectTechnique = Effect->GetTechniqueByName("TextureTechnique");
	if (!EffectTechnique)
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
	EffectTechnique->GetPassByIndex(0)->GetDesc(&passDesc);

	result = device->CreateInputLayout(polyLayout, numElements, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &InputLayout);
	if (FAILED(result)) 
	{
		MessageBox(hwnd, L"Cannot Create InputLayout", L"Error", MB_OK);
		return false;
	}

	WorldMatrixPtr = Effect->GetVariableByName("worldMatrix")->AsMatrix();
	ViewMatrixPtr = Effect->GetVariableByName("viewMatrix")->AsMatrix();
	ProjectionMatrixPtr = Effect->GetVariableByName("projectionMatrix")->AsMatrix();

	TexturePtr = Effect->GetVariableByName("shaderTexture")->AsShaderResource();

	return true;
}

void TextureShader::ReleaseShader()
{
	Utils::Release(&Effect);
	Utils::Release(&InputLayout);

	EffectTechnique           = 0;
	WorldMatrixPtr      = 0;
	ViewMatrixPtr       = 0;
	ProjectionMatrixPtr = 0;
	TexturePtr          = 0;
}

void TextureShader::HandlingError(ID3D10Blob* error, HWND hwnd, wchar_t* outFileName, wchar_t* text, wchar_t* caption)
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
	WorldMatrixPtr->SetMatrix(reinterpret_cast<float *>(&worldMatrix));
	ViewMatrixPtr->SetMatrix(reinterpret_cast<float *>(&viewMatrix));
	ProjectionMatrixPtr->SetMatrix(reinterpret_cast<float *>(&projectionMatrix));

	TexturePtr->SetResource(texture);

	return true;
}

void TextureShader::RenderShader(ID3D10Device* device, int indexCount)
{
	D3D10_TECHNIQUE_DESC techniqueDesc;

	device->IASetInputLayout(InputLayout);

	EffectTechnique->GetDesc(&techniqueDesc);

	for (int i = 0; i<techniqueDesc.Passes; ++i)
	{
		EffectTechnique->GetPassByIndex(i)->Apply(0);
		device->DrawIndexed(indexCount, 0, 0);
	}

}
