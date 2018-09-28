/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

class ShaderProgram
{
protected:
	VertexShader * mp_VertexShader;
	PixelShader * mp_PixelShader;
	ConstantBufferWrapper<MainCB>* mp_CBuffer;

public:
	ShaderProgram() {};
	~ShaderProgram() 
	{
		Release();
	};

	void Initialize(ID3D11Device*, std::string shaderFile);
	void Initialize(ID3D11Device*, std::string vertexShaderFile, std::string pixelShaderFile);
	void Initialize(ID3D11Device*, std::string vertexShaderFile, std::string pixelShaderFile, std::string vertexShaderFunc = "VShader", std::string pixelShaderFunc = "PShader");

	void BindShader();
	void Release();

	VertexShader* VS() { return mp_VertexShader; }
	PixelShader* PS() { return mp_PixelShader; }
	ConstantBufferWrapper<MainCB>* CB() { return mp_CBuffer; }
};

#endif