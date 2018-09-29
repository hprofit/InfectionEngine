/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

bool BackBufferRenderTarget::_CreateRenderViewTarget(ID3D11Device * device, IDXGISwapChain * swapChain)
{
	ID3D11Texture2D* pBuffer;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBuffer)))
		return false;

	// use the buffer address to create the render target
	if (FAILED(device->CreateRenderTargetView(pBuffer, NULL, &mp_BackBuffer)))
		return false;
	pBuffer->Release();
	return true;
}

bool BackBufferRenderTarget::_CreateDepthAndStencilBuffer(const WindowSettings& settings, ID3D11Device * device)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = settings.Width;
	depthBufferDesc.Height = settings.Height;
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
	if (FAILED(device->CreateTexture2D(&depthBufferDesc, NULL, &mp_DepthStencilBuffer)))
		return false;
	return true;
}

bool BackBufferRenderTarget::_CreateDepthStencilStates(ID3D11Device * device)
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
	if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &mp_DepthStencilState)))
		return false;

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	if (FAILED(device->CreateDepthStencilState(&depthDisabledStencilDesc, &mp_DepthDisabledStencilState)))
		return false;

	return true;
}

bool BackBufferRenderTarget::_CreateDepthStencilView(ID3D11Device * device)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	if (FAILED(device->CreateDepthStencilView(mp_DepthStencilBuffer, &depthStencilViewDesc, &mp_DepthStencilView)))
		return false;
	return true;
}

bool BackBufferRenderTarget::_CreateRasterState(ID3D11Device * device)
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
	if (FAILED(device->CreateRasterizerState(&rasterDesc, &mp_RasterState)))
		return false;
	return true;
}

bool BackBufferRenderTarget::_CreateBlendStates(ID3D11Device * device)
{
	D3D11_BLEND_DESC blendStateDescription;
	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.AlphaToCoverageEnable = TRUE;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	if (FAILED(device->CreateBlendState(&blendStateDescription, &mp_AlphaEnabledBlendingState)))
		return false;

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the second blend state using the description.
	if (FAILED(device->CreateBlendState(&blendStateDescription, &mp_AlphaDisabledBlendingState)))
		return false;
	return true;
}

BackBufferRenderTarget::BackBufferRenderTarget() :
	mp_DepthStencilBuffer(nullptr),
	mp_DepthStencilState(nullptr),
	mp_DepthStencilView(nullptr),
	mp_RasterState(nullptr),
	mp_BackBuffer(nullptr)
{}

BackBufferRenderTarget::~BackBufferRenderTarget()
{
}

void BackBufferRenderTarget::Initialize(const WindowSettings& settings, ID3D11Device * device, IDXGISwapChain * swapChain)
{
	_CreateRenderViewTarget(device, swapChain);
	_CreateDepthAndStencilBuffer(settings, device);
	_CreateDepthStencilStates(device);
	_CreateDepthStencilView(device);
	_CreateRasterState(device);

	_CreateBlendStates(device);
}

void BackBufferRenderTarget::Release()
{
	if (mp_BackBuffer)
		mp_BackBuffer->Release();
	if (mp_DepthStencilBuffer)
		mp_DepthStencilBuffer->Release();
	if (mp_DepthStencilState)
		mp_DepthStencilState->Release();
	if (mp_DepthStencilView)
		mp_DepthStencilView->Release();
	if (mp_RasterState)
		mp_RasterState->Release();
}

void BackBufferRenderTarget::BindToRead() const
{
}

void BackBufferRenderTarget::BindRenderTarget(ID3D11DeviceContext* deviceContext) const
{
	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(mp_DepthStencilState, 1);

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &mp_BackBuffer, mp_DepthStencilView);

	// Now set the rasterizer state.
	deviceContext->RSSetState(mp_RasterState);
}

void BackBufferRenderTarget::UnbindRenderTarget() const
{
}

void BackBufferRenderTarget::ClearRenderTarget(ID3D11DeviceContext * deviceContext, const Color & color)
{
	deviceContext->ClearRenderTargetView(mp_BackBuffer, color);
	deviceContext->ClearDepthStencilView(mp_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

