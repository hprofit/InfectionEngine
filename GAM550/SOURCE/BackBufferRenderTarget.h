/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef BACK_BUFFER_RENDER_TARGET_H
#define BACK_BUFFER_RENDER_TARGET_H

class BackBufferRenderTarget 
{
protected:
	ID3D11Texture2D * mp_DepthStencilBuffer;
	ID3D11DepthStencilState* mp_DepthStencilState;
	ID3D11DepthStencilState* mp_DepthDisabledStencilState;
	ID3D11DepthStencilView* mp_DepthStencilView;
	ID3D11RasterizerState* mp_RasterState;
	ID3D11RenderTargetView * mp_BackBuffer;
	ID3D11BlendState * mp_AlphaEnabledBlendingState;
	ID3D11BlendState * mp_AlphaDisabledBlendingState;

	virtual bool _CreateRenderViewTarget(ID3D11Device * device, IDXGISwapChain * swapChain);
	virtual bool _CreateDepthAndStencilBuffer(const WindowSettings& settings, ID3D11Device * device);
	virtual bool _CreateDepthStencilStates(ID3D11Device * device);
	virtual bool _CreateDepthStencilView(ID3D11Device * device);
	virtual bool _CreateRasterState(ID3D11Device * device);
	virtual bool _CreateBlendStates(ID3D11Device * device);
public:
	BackBufferRenderTarget();
	virtual ~BackBufferRenderTarget();

	inline ID3D11Texture2D* DepthStencilBuffer() { return mp_DepthStencilBuffer; }
	inline ID3D11DepthStencilState* DepthStencilState() { return mp_DepthStencilState; }
	inline ID3D11DepthStencilState* DepthDisabledStencilState() { return mp_DepthDisabledStencilState; }
	inline ID3D11DepthStencilView* DepthStencilView() { return mp_DepthStencilView; }
	inline ID3D11RasterizerState* RasterState() { return mp_RasterState; }
	inline ID3D11RenderTargetView* RenderTargetView() { return mp_BackBuffer; }
	inline ID3D11BlendState* AlphaEnabledBlendingState() { return mp_AlphaEnabledBlendingState; }
	inline ID3D11BlendState* AlphaDisabledBlendingState() { return mp_AlphaDisabledBlendingState; }

	void Initialize(const WindowSettings&, ID3D11Device*, IDXGISwapChain *);

	void Release();

	/*
		Binds this RenderTarget, setting it as the current render target. Anything
		rendered after this method will be rendered to this RenderTarget
		and not to the screen.
	*/
	void BindRenderTarget(ID3D11DeviceContext* deviceContext) const;

	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, const Color& color = Color(0, 0, 0, 1));
};

#endif