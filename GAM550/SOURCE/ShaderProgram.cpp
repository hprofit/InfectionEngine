/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

void ShaderProgram::Initialize(ID3D11Device* device, std::string shaderFile)
{
	mp_VertexShader = new VertexShader(shaderFile);
	mp_PixelShader = new PixelShader(shaderFile);
	mp_CBuffer = new ConstantBufferWrapper<MainCB>(device);
}

void ShaderProgram::Initialize(ID3D11Device* device, std::string vertexShaderFile, std::string pixelShaderFile)
{
	mp_VertexShader = new VertexShader(vertexShaderFile);
	mp_PixelShader = new PixelShader(pixelShaderFile);
	mp_CBuffer = new ConstantBufferWrapper<MainCB>(device);
}

void ShaderProgram::Initialize(ID3D11Device* device, std::string vertexShaderFile, std::string pixelShaderFile, std::string vertexShaderFunc, std::string pixelShaderFunc)
{
	mp_VertexShader = new VertexShader(vertexShaderFile, vertexShaderFunc);
	mp_PixelShader = new PixelShader(pixelShaderFile, pixelShaderFunc);
	mp_CBuffer = new ConstantBufferWrapper<MainCB>(device);
}

void ShaderProgram::BindShader()
{
	mp_VertexShader->BindShader();
	mp_PixelShader->BindShader();
}

void ShaderProgram::Release()
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
