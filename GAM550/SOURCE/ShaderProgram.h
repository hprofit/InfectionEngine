/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

template <typename CBufferType>
class ShaderProgram
{
protected:
	VertexShader * mp_VertexShader;
    ComputeShader * mp_ComputeShader;
	PixelShader * mp_PixelShader;
	ConstantBufferWrapper<CBufferType> * mp_CBuffer;

public:
	ShaderProgram() {};
	~ShaderProgram() 
	{
		Release();
	};

    struct ShaderMetaData {
        std::string m_sShaderFile;
        std::string m_sShaderFunc;
        Shader::ShaderType m_eShaderType;
    };

    void Initialize(ID3D11Device*, std::vector<ShaderMetaData> a_shaderMetaData);
	void Initialize(ID3D11Device*, std::string shaderFile);
	void Initialize(ID3D11Device*, std::string vertexShaderFile, std::string pixelShaderFile);
	void Initialize(ID3D11Device*, std::string vertexShaderFile, std::string pixelShaderFile, std::string vertexShaderFunc = "VShader", std::string pixelShaderFunc = "PShader");

	// Bind this shader program for use
	void BindShader();
	// Releases all shaders and constant buffers before setting pointers to null
	void Release();

	VertexShader* VS() { return mp_VertexShader; }
    ComputeShader* CS() { return mp_ComputeShader; }
	PixelShader* PS() { return mp_PixelShader; }
	// Returns a pointer to this ShaderProgram's constant buffer
	ConstantBufferWrapper<CBufferType>* CB() { return mp_CBuffer; }
};


template<typename CBufferType>
inline void ShaderProgram<CBufferType>::Initialize(ID3D11Device * device, std::vector<ShaderMetaData> a_shaderMetaData)
{
    for (ShaderMetaData data : a_shaderMetaData) 
    {
        switch (data.m_eShaderType) {
            case Shader::ShaderType_Vertex:
            {
                mp_VertexShader = new VertexShader(data.m_sShaderFile, data.m_sShaderFunc);
                break;
            }
            case Shader::ShaderType_Compute:
            {
                mp_ComputeShader = new ComputeShader(data.m_sShaderFile, data.m_sShaderFunc);
                break;
            }
            case Shader::ShaderType_Pixel:
            {
                mp_PixelShader = new PixelShader(data.m_sShaderFile, data.m_sShaderFunc);
                break;
            }
        }
    }
    mp_CBuffer = new ConstantBufferWrapper<CBufferType>(device);
}

template <typename CBufferType>
void ShaderProgram<CBufferType>::Initialize(ID3D11Device* device, std::string shaderFile)
{
	mp_VertexShader = new VertexShader(shaderFile);
	mp_PixelShader = new PixelShader(shaderFile);
	mp_CBuffer = new ConstantBufferWrapper<CBufferType>(device);
}

template <typename CBufferType>
void ShaderProgram<CBufferType>::Initialize(ID3D11Device* device, std::string vertexShaderFile, std::string pixelShaderFile)
{
	mp_VertexShader = new VertexShader(vertexShaderFile);
	mp_PixelShader = new PixelShader(pixelShaderFile);
	mp_CBuffer = new ConstantBufferWrapper<CBufferType>(device);
}

template <typename CBufferType>
void ShaderProgram<CBufferType>::Initialize(ID3D11Device* device, std::string vertexShaderFile, std::string pixelShaderFile, std::string vertexShaderFunc, std::string pixelShaderFunc)
{
	mp_VertexShader = new VertexShader(vertexShaderFile, vertexShaderFunc);
	mp_PixelShader = new PixelShader(pixelShaderFile, pixelShaderFunc);
	mp_CBuffer = new ConstantBufferWrapper<CBufferType>(device);
}

template <typename CBufferType>
void ShaderProgram<CBufferType>::BindShader()
{
    if (mp_VertexShader)
	    mp_VertexShader->BindShader();
    if (mp_ComputeShader)
        mp_ComputeShader->BindShader();
    if (mp_PixelShader)
	    mp_PixelShader->BindShader();
}

template <typename CBufferType>
void ShaderProgram<CBufferType>::Release()
{
	if (mp_VertexShader)
		mp_VertexShader->Release();
	if (mp_PixelShader)
		mp_PixelShader->Release();
	if (mp_CBuffer)
		mp_CBuffer->Release();


	mp_VertexShader = nullptr;
	mp_PixelShader = nullptr;
	mp_CBuffer = nullptr;
}




#endif