/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

struct ConstantBuffer {};

struct MainCB : public ConstantBuffer {
	Matrix4x4 MatFinal;
	Matrix4x4 ModelMatrix;
	Matrix4x4 NormalMatrix;
	Vector3D CameraPosition;
	Color OverrideColor;
	Color SpecularValues;
	int CastShadows;
	int ReceiveShadows;
	int IsLit;
	unsigned int TextureFlags;
};

struct QuadCB : public ConstantBuffer {
	Matrix4x4 ModelMatrix;
	Color Ambient;
};

struct DeferredFinalCB : public ConstantBuffer {
	Matrix4x4 MatFinal;
	Matrix4x4 ModelMatrix;
	Vector3D CameraPosition;
	Vector3D LightPosition;
	Color LightColor;
	Vector3D LIDHW;	// Light Intensity, Light Distance, Screen Height, Screen Width
	Color Ambient;
};

struct ShadowCB : public ConstantBuffer {
	Matrix4x4 MatFinal;
};

struct ShadowAddLightCB : public ConstantBuffer {
	Matrix4x4 ShadowMatrix;
	Matrix4x4 ModelMatrix;
	Vector3D CameraPosition;
	Vector3D LightPosition;	// x, y, z, A
	Color LightColor;		// r, g, b, B
	Vector3D LIDHW;	// Intensity, Distance, Render Target Height, Render Target Width
};

class CBW {
protected:
	ID3D11Buffer* mp_CBuffer;	// Pointer to DirectX Constant Buffer object

public:
	CBW();
	virtual ~CBW();

	virtual void SetConstantBuffer(ID3D11DeviceContext* deviceContext);
	virtual void UpdateSubresource(ID3D11DeviceContext* deviceContext) = 0;
	virtual void Release();
};

template<typename BufferType>
class ConstantBufferWrapper : 
	public CBW
{
protected:
	BufferType mp_CBufferData;

public:
	ConstantBufferWrapper(ID3D11Device* device);
	virtual ~ConstantBufferWrapper() {};

	ID3D11Buffer* CBuffer() { return mp_CBuffer; }
	BufferType& BufferData() { return mp_CBufferData; }

	virtual void UpdateSubresource(ID3D11DeviceContext* deviceContext);
};

template<typename BufferType>
ConstantBufferWrapper<BufferType>::ConstantBufferWrapper(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(BufferType) + (16 - (sizeof(BufferType) % 16));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	device->CreateBuffer(&bd, NULL, &mp_CBuffer);
}

template<typename BufferType>
void ConstantBufferWrapper<BufferType>::UpdateSubresource(ID3D11DeviceContext* deviceContext)
{
	deviceContext->UpdateSubresource(mp_CBuffer, 0, 0, &mp_CBufferData, 0, 0);
}



#endif