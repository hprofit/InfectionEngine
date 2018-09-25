/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ShaderProgram::ShaderProgram(std::string shaderFile)
{
	mp_VertexShader = new VertexShader(shaderFile);
	mp_PixelShader = new PixelShader(shaderFile);
	mp_CBuffer = new ConstantBufferWrapper<MainCB>(INFECT_RENDERER.Device());
}

ShaderProgram::ShaderProgram(std::string vertexShaderFile, std::string pixelShaderFile)
{
	mp_VertexShader = new VertexShader(vertexShaderFile);
	mp_PixelShader = new PixelShader(pixelShaderFile);
	mp_CBuffer = new ConstantBufferWrapper<MainCB>(INFECT_RENDERER.Device());
}

ShaderProgram::ShaderProgram(std::string vertexShaderFile, std::string pixelShaderFile, std::string vertexShaderFunc, std::string pixelShaderFunc)
{
	mp_VertexShader = new VertexShader(vertexShaderFile, vertexShaderFunc);
	mp_PixelShader = new PixelShader(pixelShaderFile, pixelShaderFunc);
	mp_CBuffer = new ConstantBufferWrapper<MainCB>(INFECT_RENDERER.Device());
}

ShaderProgram::~ShaderProgram(){}

void ShaderProgram::BindShader()
{
	mp_VertexShader->BindShader();
	mp_PixelShader->BindShader();
}

void ShaderProgram::Release()
{
	mp_VertexShader->Release();
	mp_PixelShader->Release();
	mp_CBuffer->Release();
}
