/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef D3DHANDLER_H
#define D3DHANDLER_H

class D3DHandler
{
protected:
	IDXGISwapChain *mp_SwapChain;
	ID3D11Device *mp_Device;
	ID3D11DeviceContext *mp_DeviceContext;

	BackBufferRenderTarget* mp_BackBuffer;	// <- move these render targets to the RenderManager
	RenderTarget* mp_DeferredRenderTarget;	// <- move these render targets to the RenderManager
	RenderTarget* mp_SecondPassRenderTarget;// <- move these render targets to the RenderManager
	RenderTarget* mp_LightRenderTarget;		// <- move these render targets to the RenderManager
	D3D11_VIEWPORT m_viewport;

	ID3D11RasterizerState* mp_RasterStateBackCulling;
	ID3D11RasterizerState* mp_RasterStateFrontCulling;

	int m_VideoCardMemory;
	char m_VideoCardDescription[128];

	bool _CreateRasterStates();
public:
	friend class RenderManager;
	D3DHandler();
	~D3DHandler();

	inline IDXGISwapChain * const SwapChain() { return mp_SwapChain; }
	inline ID3D11Device * const Device() { return mp_Device; }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_DeviceContext; }

	// Sets up and initializes window
	HWND InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings);
	// Sets up and initializes Direct3D
	bool InitD3D(HWND hWnd, WindowSettings settings);
	// Closes Direct3D and releases memory
	void CleanD3D(void);

	RenderTarget* GetDeferredRenderTarget() { return mp_DeferredRenderTarget; }
	RenderTarget* GetSecondPassRenderTarget() { return mp_SecondPassRenderTarget; }
	RenderTarget* GetLightRT() { return mp_LightRenderTarget; }

	void BindBackBuffer() const;

	void BindDeferredBuffer() const;

	void BindSecondPassBuffer() const;

	// Clear the currently bound render target and the depth buffer
	void ClearBackBuffer(const Color& color);

	void ClearDeferredBuffer(const Color& color);

	void ClearSecondPassBuffer(const Color& color);

	void ClearLightBuffer(const Color& color);


	void PresentBuffer(bool vSync);

	void EnableAlpha();

	void DisableAlpha();

	void EnableDepth();

	void DisableDepth();

	void EnableFrontFaceCulling();

	void EnableBackFaceCulling();
};

#endif