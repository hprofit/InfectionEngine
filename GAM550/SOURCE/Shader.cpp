/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

VertexShader::VertexShader(std::string shaderFilePath) :
	Shader()
{
	LoadFromFile(shaderFilePath);
}

VertexShader::VertexShader(std::string shaderFilePath, std::string shaderFunc)
{
}

bool VertexShader::LoadFromFile(std::string shaderFilePath, std::string shaderFunc)
{
	// load and compile the shaders
	int flag = D3D10_SHADER_WARNINGS_ARE_ERRORS;// | D3D10_SHADER_OPTIMIZATION_LEVEL3;
	ID3DBlob * pErrors;

	HRESULT result = D3DCompileFromFile(std::wstring(shaderFilePath.begin(), shaderFilePath.end()).c_str(), 0, 0, shaderFunc.c_str(), "vs_4_0", flag, flag, &mp_ShaderBlob, &pErrors);
	if (FAILED(result)) {
		MessageBox(NULL, "The vertex shader failed to compile.", "Error", MB_OK);
		return false;
	}

	// Encapsulate the shaders into a shader object
	result = INFECT_RENDERER.Device()->CreateVertexShader(mp_ShaderBlob->GetBufferPointer(), mp_ShaderBlob->GetBufferSize(), NULL, &mp_VS);
	if (FAILED(result)) {
		MessageBox(NULL, "The vertex shader failed to be created.", "Error", MB_OK);
		return false;
	}

	return true;
}

void VertexShader::BindShader() const
{
	INFECT_RENDERER.DeviceContext()->VSSetShader(mp_VS, 0, 0);
}

void VertexShader::Release()
{
	mp_VS->Release();
}






PixelShader::PixelShader(std::string shaderFilePath) :
	Shader()
{
	LoadFromFile(shaderFilePath);
}

PixelShader::PixelShader(std::string shaderFilePath, std::string shaderFunc)
{
}

bool PixelShader::LoadFromFile(std::string shaderFilePath, std::string shaderFunc)
{
	// load and compile the shaders
	int flag = D3D10_SHADER_WARNINGS_ARE_ERRORS;// | D3D10_SHADER_OPTIMIZATION_LEVEL3;
	ID3DBlob * pErrors;

	HRESULT result = D3DCompileFromFile(std::wstring(shaderFilePath.begin(), shaderFilePath.end()).c_str(), 0, 0, shaderFunc.c_str(), "ps_4_0", flag, flag, &mp_ShaderBlob, &pErrors);
	if (FAILED(result)) {
		MessageBox(NULL, "The pixel shader failed to compile.", "Error", MB_OK);
		return false;
	}

	// Encapsulate the shaders into a shader object
	result = INFECT_RENDERER.Device()->CreatePixelShader(mp_ShaderBlob->GetBufferPointer(), mp_ShaderBlob->GetBufferSize(), NULL, &mp_PS);
	if (FAILED(result)) {
		MessageBox(NULL, "The pixel shader failed to be created.", "Error", MB_OK);
		return false;
	}

	return true;
}

void PixelShader::BindShader() const
{
	INFECT_RENDERER.DeviceContext()->PSSetShader(mp_PS, 0, 0);
}

void PixelShader::Release()
{
	mp_PS->Release();
}
