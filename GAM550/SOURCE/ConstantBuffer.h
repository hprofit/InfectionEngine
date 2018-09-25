/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef CONSTANT_BUFFER_H
#define CONSTANT_BUFFER_H

struct ConstantBuffer {

};

struct MainCB : public ConstantBuffer {
	Matrix4x4 MatFinal;
	Matrix4x4 ModelMatrix;
	Matrix4x4 NormalMatrix;
	Vector3D CameraPosition;
	int CastShadows;
	int ReceiveShadows;
	int IsLit;
	int Textured;
};

template<typename BufferType>
class ConstantBufferWrapper {
protected:
	ID3D11Buffer* mp_CBuffer;	// Pointer to DirectX Constant Buffer object
	BufferType mp_CBufferData;

public:
	ConstantBufferWrapper(ID3D11Device* device);
	virtual ~ConstantBufferWrapper() {};

	ID3D11Buffer* CBuffer() { return mp_CBuffer; }
	BufferType& BufferData() { return mp_CBufferData; }

	void SetConstantBuffer(ID3D11DeviceContext* deviceContext);
	void UpdateSubresource(ID3D11DeviceContext* deviceContext);
	void Release();
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
void ConstantBufferWrapper<BufferType>::SetConstantBuffer(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetConstantBuffers(0, 1, &(mp_CBuffer));
	deviceContext->PSSetConstantBuffers(0, 1, &mp_CBuffer);
}

template<typename BufferType>
void ConstantBufferWrapper<BufferType>::UpdateSubresource(ID3D11DeviceContext* deviceContext)
{
	deviceContext->UpdateSubresource(mp_CBuffer, 0, 0, &mp_CBufferData, 0, 0);
}

template<typename BufferType>
void ConstantBufferWrapper<BufferType>::Release()
{
	if (mp_CBuffer)
		mp_CBuffer->Release();
}


#endif