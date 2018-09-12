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
	IDXGISwapChain * mp_SwapChain;
	ID3D11Device *mp_Device;
	ID3D11DeviceContext *mp_DeviceContext;
	ID3D11RenderTargetView *mp_BackBuffer, *mp_DepthBuffer;

public:
	D3DHandler();
	~D3DHandler();

	inline IDXGISwapChain * const SwapChain() { return mp_SwapChain; }
	inline ID3D11Device * const Device() { return mp_Device; }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_DeviceContext; }
	inline ID3D11RenderTargetView * const BackBuffer() { return mp_BackBuffer; }
	inline ID3D11RenderTargetView * const DepthBuffer() { return mp_DepthBuffer; }

	// Creates a console for output
	void InitConsole();
	// Destroys the active console
	void DestroyConsole();
	// Sets up and initializes window
	void InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings);
	// Sets up and initializes Direct3D
	void InitD3D(HWND hWnd, WindowSettings settings);
	// Closes Direct3D and releases memory
	void CleanD3D(void);
};

#endif