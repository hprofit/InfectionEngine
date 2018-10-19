/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

class RenderTarget
{
protected:
	ID3D11Texture2D * mp_DepthStencilBuffer;
	ID3D11DepthStencilState* mp_DepthStencilState;
	ID3D11DepthStencilView* mp_DepthStencilView;
	unsigned int m_TextureWidth, m_TextureHeight, m_NumTargets;

	ID3D11Texture2D** m_renderTargetTextures;
	ID3D11RenderTargetView** m_renderTargetViews;
	ID3D11ShaderResourceView** m_shaderResourceViews;
	D3D11_VIEWPORT m_viewport;


	virtual bool _CreateRenderViewTarget(ID3D11Device * device, unsigned int);
	virtual bool _CreateDepthAndStencilBuffer(ID3D11Device * device);
	virtual bool _CreateDepthStencilState(ID3D11Device * device);
	virtual bool _CreateDepthStencilView(ID3D11Device * device);
public:
	RenderTarget(unsigned int width, unsigned int height, unsigned int numTargets = 1);
	virtual ~RenderTarget();

	inline ID3D11Texture2D* DepthStencilBuffer() { return mp_DepthStencilBuffer; }
	inline ID3D11DepthStencilState* DepthStencilState() { return mp_DepthStencilState; }
	inline ID3D11DepthStencilView* DepthStencilView() { return mp_DepthStencilView; }
    inline D3D11_VIEWPORT ViewPort() { return m_viewport; }

	inline unsigned int GetNumViews() const { return m_NumTargets; }
	inline ID3D11Texture2D** GetRenderTextures() { return m_renderTargetTextures; }
	inline ID3D11ShaderResourceView** GetShaderResourceViews() { return m_shaderResourceViews; }
    inline ID3D11RenderTargetView** GetRenderTargetViews() { return m_renderTargetViews; }

	void Initialize(ID3D11Device*);

	void Release();

	/*
		Binds this RenderTarget, setting it as the current render target. Anything
		rendered after this method will be rendered to this RenderTarget
		and not to the screen.
	*/
	void BindRenderTarget(ID3D11DeviceContext* deviceContext) const;

	// Binds a single RenderTargetView, does NOT do index checking to prevent bad access
	// Sets the DepthStencilState
	void BindRT(ID3D11DeviceContext* deviceContext, UINT idx) const;

	void SetViewport(ID3D11DeviceContext* deviceContext) const;

	// Clears all render targets and the depth stencil view
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, const Color& color = Color(0,0,0,1));

	// Clears just the depth stencil view
	void ClearDepthStencilView(ID3D11DeviceContext* deviceContext);
};

#endif