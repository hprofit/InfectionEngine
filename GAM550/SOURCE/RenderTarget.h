/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

enum RenderTargetType {
	FBO_NONE = 0,
	FBO_DEPTH_RENDER_BUFFER,
	FBO_DEPTH_TEXTURE
};

class RenderTarget
{
protected:
	ID3D11Texture2D * mp_DepthAndStencilBuffer;
	ID3D11DepthStencilState* mp_DepthStencilState;
	ID3D11DepthStencilView* mp_DepthStencilView;
	ID3D11RasterizerState* mp_RasterState;

	ID3D11RenderTargetView * mp_BackBuffer;

	bool _CreateRenderViewTarget();
	bool _CreateDepthAndStencilBuffer(unsigned int width, unsigned int height);
	bool _CreateDepthStencilState();
	bool _CreateDepthStencilView();
	bool _CreateRasterState();
public:
	RenderTarget(unsigned int width, unsigned int height);
	~RenderTarget();

	inline ID3D11Texture2D* DepthStencilBuffer() { return mp_DepthAndStencilBuffer; }
	inline ID3D11DepthStencilState* DepthStencilState() { return mp_DepthStencilState; }
	inline ID3D11DepthStencilView* DepthStencilView() { return mp_DepthStencilView; }
	inline ID3D11RasterizerState* RasterState() { return mp_RasterState; }
	inline ID3D11RenderTargetView* RenderTargetView() { return mp_BackBuffer; }

	void Release();

	// Binds the current FBO to be read from
	void BindToRead() const;

	/*
		Binds this RenderTarget, setting it as the current render target. Anything
		rendered after this method will be rendered to this RenderTarget
		and not to the screen.
	*/
	void BindRenderTarget() const;

	/*
		Unbinds this RenderTarget, setting the default RenderTarget as the current
		render target. Anything rendered after this will be rendered to the
		screen, not this RenderTarget.
	*/
	void UnbindRenderTarget() const;

	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, const Color& color = Color(0,0,0,1));
};

#endif