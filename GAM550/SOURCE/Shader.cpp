/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

UINT Shader::_GetFlags()
{
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
    flags |= D3D10_SHADER_WARNINGS_ARE_ERRORS;
#else
    flags |= D3D10_SHADER_OPTIMIZATION_LEVEL3;
#endif
    return flags;
}

LPCSTR Shader::_GetTarget(ShaderType a_Type)
{
    switch (a_Type) {
    case ShaderType::ShaderType_Vertex:
        return INFECT_RENDERER.Device()->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "vs_5_0" : "vs_4_0";
        break;
    case ShaderType::ShaderType_Compute:
        return INFECT_RENDERER.Device()->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "cs_5_0" : "cs_4_0";
        break;
    case ShaderType::ShaderType_Pixel:
        return INFECT_RENDERER.Device()->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "ps_5_0" : "ps_4_0";
        break;
    default:
        return nullptr;
        break;
    }
}



VertexShader::VertexShader(std::string shaderFilePath) :
	Shader()
{
	LoadFromFile(shaderFilePath);
}

VertexShader::VertexShader(std::string shaderFilePath, std::string shaderFunc) : Shader()
{
    LoadFromFile(shaderFilePath, shaderFunc);
}

bool VertexShader::LoadFromFile(std::string shaderFilePath, std::string shaderFunc)
{
	// load and compile the shaders
	UINT flags = _GetFlags();
    LPCSTR target = _GetTarget(ShaderType::ShaderType_Vertex);
	ID3DBlob * pErrors;

	HRESULT result = D3DCompileFromFile(std::wstring(shaderFilePath.begin(), shaderFilePath.end()).c_str(), 0, 0, shaderFunc.c_str(), target, flags, flags, &mp_ShaderBlob, &pErrors);
	if (FAILED(result)) {
		CreateErrorDialog(pErrors, "The vertex shader failed to compile:\n ");
		return false;
	}

	// Encapsulate the shaders into a shader object
	result = INFECT_RENDERER.Device()->CreateVertexShader(mp_ShaderBlob->GetBufferPointer(), mp_ShaderBlob->GetBufferSize(), nullptr, &mp_VS);
	if (FAILED(result)) {
		CreateErrorDialog(nullptr, "The vertex shader failed to be created.");
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
	if (mp_VS) {
		mp_VS->Release();
		mp_VS = nullptr;
	}
}




ComputeShader::ComputeShader(std::string shaderFilePath) :
    Shader()
{
    LoadFromFile(shaderFilePath);
}

ComputeShader::ComputeShader(std::string shaderFilePath, std::string shaderFunc) :
    Shader()
{
    LoadFromFile(shaderFilePath, shaderFunc);
}

bool ComputeShader::LoadFromFile(std::string shaderFilePath, std::string shaderFunc)
{
    // load and compile the shaders
    UINT flags = _GetFlags();
    LPCSTR target = _GetTarget(ShaderType::ShaderType_Compute);
    ID3DBlob * pErrors;

    HRESULT result = D3DCompileFromFile(std::wstring(shaderFilePath.begin(), shaderFilePath.end()).c_str(), 0, 0, shaderFunc.c_str(), target, flags, flags, &mp_ShaderBlob, &pErrors);
    if (FAILED(result)) {
        CreateErrorDialog(pErrors, "The compute shader failed to compile:\n ");
        return false;
    }

    // Encapsulate the shaders into a shader object
    result = INFECT_RENDERER.Device()->CreateComputeShader(mp_ShaderBlob->GetBufferPointer(), mp_ShaderBlob->GetBufferSize(), nullptr, &mp_CS);
    if (FAILED(result)) {
        CreateErrorDialog(nullptr, "The compute shader failed to be created.");
        return false;
    }

    return true;
}

void ComputeShader::BindShader() const
{
    INFECT_RENDERER.DeviceContext()->CSSetShader(mp_CS, 0, 0);
}

void ComputeShader::Release()
{
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
    UINT flags = _GetFlags();
    LPCSTR target = _GetTarget(ShaderType::ShaderType_Pixel);
	ID3DBlob * pErrors;

	HRESULT result = D3DCompileFromFile(std::wstring(shaderFilePath.begin(), shaderFilePath.end()).c_str(), 0, 0, shaderFunc.c_str(), target, flags, flags, &mp_ShaderBlob, &pErrors);
	if (FAILED(result)) {
		CreateErrorDialog(pErrors, "The pixel shader failed to compile:\n ");
		return false;
	}

	// Encapsulate the shaders into a shader object
	result = INFECT_RENDERER.Device()->CreatePixelShader(mp_ShaderBlob->GetBufferPointer(), mp_ShaderBlob->GetBufferSize(), nullptr, &mp_PS);
	if (FAILED(result)) {
		CreateErrorDialog(nullptr, "The pixel shader failed to be created.");
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
	if (mp_PS) {
		mp_PS->Release();
		mp_PS = nullptr;
	}
}
