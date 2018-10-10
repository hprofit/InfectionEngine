/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

bool RenderTarget::_CreateRenderViewTarget(ID3D11Device * device, unsigned int index)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target textures.
	if (FAILED( device->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTextures[index])))
		return false;

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateRenderTargetView(m_renderTargetTextures[index], &renderTargetViewDesc, &m_renderTargetViews[index])))
		return false;

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource views.
	if (FAILED(device->CreateShaderResourceView(m_renderTargetTextures[index], &shaderResourceViewDesc, &m_shaderResourceViews[index])))
		return false;
	return true;
}

bool RenderTarget::_CreateDepthAndStencilBuffer(ID3D11Device * device)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_TextureWidth;
	depthBufferDesc.Height = m_TextureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; //DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	
	// Create the texture for the depth buffer using the filled out description.
	if (FAILED(device->CreateTexture2D(&depthBufferDesc, NULL, &mp_DepthStencilBuffer)))
		return false;
	return true;
}

bool RenderTarget::_CreateDepthStencilState(ID3D11Device * device)
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
	return true;
}

bool RenderTarget::_CreateDepthStencilView(ID3D11Device * device)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	if (FAILED(device->CreateDepthStencilView(mp_DepthStencilBuffer, &depthStencilViewDesc, &mp_DepthStencilView)))
		return false;
	return true;
}

bool RenderTarget::_CreateRasterState(ID3D11Device * device)
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

RenderTarget::RenderTarget(unsigned int width, unsigned int height, unsigned int numTargets) :
	m_TextureWidth(width), m_TextureHeight(height), m_NumTargets(numTargets)
{
	m_renderTargetTextures = new ID3D11Texture2D*[m_NumTargets];
	m_renderTargetViews = new ID3D11RenderTargetView*[m_NumTargets];
	m_shaderResourceViews = new ID3D11ShaderResourceView*[m_NumTargets];

	for (unsigned int i = 0; i < m_NumTargets; ++i) {
		m_renderTargetTextures[i] = nullptr;
		m_renderTargetViews[i] = nullptr;
		m_shaderResourceViews[i] = nullptr;
	}
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Initialize(ID3D11Device * device)
{
	for (unsigned int i = 0; i < m_NumTargets; ++i) {
		_CreateRenderViewTarget(device, i);
	}

	_CreateDepthAndStencilBuffer(device);
	_CreateDepthStencilState(device);
	_CreateDepthStencilView(device);
	_CreateRasterState(device);

	// Setup the viewport for rendering.
	m_viewport.Width = (float)m_TextureWidth;
	m_viewport.Height = (float)m_TextureHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
}

void RenderTarget::Release()
{
	if (mp_DepthStencilBuffer)
		mp_DepthStencilBuffer->Release();
	if (mp_DepthStencilState)
		mp_DepthStencilState->Release();
	if (mp_DepthStencilView)
		mp_DepthStencilView->Release();
	if (mp_RasterState)
		mp_RasterState->Release();


	for (unsigned int i = 0; i < m_NumTargets; ++i) {
		if (m_renderTargetTextures[i])
			m_renderTargetTextures[i]->Release();
		if (m_renderTargetViews[i])
			m_renderTargetViews[i]->Release();
		if (m_shaderResourceViews[i])
			m_shaderResourceViews[i]->Release();
		m_renderTargetTextures[i] = nullptr;
		m_renderTargetViews[i] = nullptr;
		m_shaderResourceViews[i] = nullptr;
	}
	delete[] m_renderTargetTextures;
	delete[] m_renderTargetViews;
	delete[] m_shaderResourceViews;
}

void RenderTarget::BindToRead() const
{

}

void RenderTarget::BindRenderTarget(ID3D11DeviceContext* deviceContext) const
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(m_NumTargets, m_renderTargetViews, mp_DepthStencilView);

	// Now set the rasterizer state.
	deviceContext->RSSetState(mp_RasterState);

	// Set the depth stencil state.
	deviceContext->OMSetDepthStencilState(mp_DepthStencilState, 1);

	deviceContext->RSSetViewports(1, &m_viewport);
}

void RenderTarget::UnbindRenderTarget() const
{
	
}

void RenderTarget::ClearRenderTarget(ID3D11DeviceContext* deviceContext, const Color& color)
{
	for (unsigned int i = 0; i < m_NumTargets; ++i) {
		deviceContext->ClearRenderTargetView(m_renderTargetViews[i], color);
	}
	deviceContext->ClearDepthStencilView(mp_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
