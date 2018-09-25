/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef SHADER_H
#define SHADER_H

class Shader {
protected:
	ID3DBlob * mp_ShaderBlob;
public:
	Shader() {};
	virtual ~Shader() {}
	
	virtual void BindShader() const = 0;
	virtual void Release() = 0;
	ID3DBlob* Blob() const { return mp_ShaderBlob; }
};



class VertexShader : 
	public Shader
{
protected:
	ID3D11VertexShader * mp_VS;		// the vertex shader

public:
	VertexShader(std::string shaderFilePath);
	VertexShader(std::string shaderFilePath, std::string shaderFunc);
	virtual ~VertexShader() {};

	bool LoadFromFile(std::string shaderFilePath, std::string shaderFunc = "VShader");
	ID3D11VertexShader* VS() const { return mp_VS; }
	virtual void BindShader() const;
	virtual void Release();
};



class PixelShader :
	public Shader
{
protected:
	ID3D11PixelShader * mp_PS;		// the pixel shader

public:
	PixelShader(std::string shaderFilePath);
	PixelShader(std::string shaderFilePath, std::string shaderFunc);
	virtual ~PixelShader() {};

	bool LoadFromFile(std::string shaderFilePath, std::string shaderFunc = "PShader");
	ID3D11PixelShader* PS() const { return mp_PS; }
	virtual void BindShader() const;
	virtual void Release();
};

#endif