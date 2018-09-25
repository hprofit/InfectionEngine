/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

bool RenderTarget::_CreateRenderViewTarget()
{
	ID3D11Texture2D* pBuffer;
	HRESULT result = INFECT_RENDERER.SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBuffer);
	if (FAILED(result))
		return false;

	// use the buffer address to create the render target
	result = INFECT_RENDERER.Device()->CreateRenderTargetView(pBuffer, NULL, &mp_BackBuffer);
	if (FAILED(result))
		return false;
	pBuffer->Release();
	return true;
}

bool RenderTarget::_CreateDepthAndStencilBuffer(unsigned int width, unsigned int height)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	HRESULT result = INFECT_RENDERER.Device()->CreateTexture2D(&depthBufferDesc, NULL, &mp_DepthAndStencilBuffer);
	return FAILED(result) ? false : true;
}

bool RenderTarget::_CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	HRESULT result = INFECT_RENDERER.Device()->CreateDepthStencilState(&depthStencilDesc, &mp_DepthStencilState);
	if (FAILED(result))
		return false;
	return true;
}

bool RenderTarget::_CreateDepthStencilView()
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	HRESULT result = INFECT_RENDERER.Device()->CreateDepthStencilView(mp_DepthAndStencilBuffer, &depthStencilViewDesc, &mp_DepthStencilView);
	if (FAILED(result))
		return false;
	return true;
}

bool RenderTarget::_CreateRasterState()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HRESULT result = INFECT_RENDERER.Device()->CreateRasterizerState(&rasterDesc, &mp_RasterState);
	if (FAILED(result))
		return false;
	return true;
}

RenderTarget::RenderTarget(unsigned int width, unsigned int height)
{
	_CreateRenderViewTarget();
	_CreateDepthAndStencilBuffer(width, height);
	_CreateDepthStencilState();
	_CreateDepthStencilView();
	_CreateRasterState();
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Release()
{
	if (mp_BackBuffer)
		mp_BackBuffer->Release();

	if (mp_DepthAndStencilBuffer)
		mp_DepthAndStencilBuffer->Release();
	if (mp_DepthStencilState)
		mp_DepthStencilState->Release();
	if (mp_DepthStencilView)
		mp_DepthStencilView->Release();
	if (mp_RasterState)
		mp_RasterState->Release();
}

void RenderTarget::BindToRead() const
{

}

void RenderTarget::BindRenderTarget() const
{
	// Set the depth stencil state.
	INFECT_RENDERER.DeviceContext()->OMSetDepthStencilState(mp_DepthStencilState, 1);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	INFECT_RENDERER.DeviceContext()->OMSetRenderTargets(1, &mp_BackBuffer, mp_DepthStencilView);
	INFECT_RENDERER.DeviceContext()->RSSetState(mp_RasterState);

	// Now set the rasterizer state.
	INFECT_RENDERER.DeviceContext()->RSSetState(mp_RasterState);
}

void RenderTarget::UnbindRenderTarget() const
{
	
}

void RenderTarget::ClearRenderTarget(const Color& color)
{
	
}
