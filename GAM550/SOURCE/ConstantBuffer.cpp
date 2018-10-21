/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include "Stdafx.h"

CBW::CBW()
{
}

CBW::~CBW()
{
	Release();
}

void CBW::SetConstantBuffer(ID3D11DeviceContext * deviceContext)
{
	deviceContext->VSSetConstantBuffers(0, 1, &mp_CBuffer);
    deviceContext->CSSetConstantBuffers(0, 1, &mp_CBuffer);
	deviceContext->PSSetConstantBuffers(0, 1, &mp_CBuffer);
}

void CBW::Release()
{
	if (mp_CBuffer)
		mp_CBuffer->Release();
	mp_CBuffer = nullptr;
}
