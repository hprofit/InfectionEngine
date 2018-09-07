#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

class RenderManager : public Subscriber
{
private:
	IDXGISwapChain *mp_SwapChain;
	ID3D11Device *mp_Device;
	ID3D11DeviceContext *mp_DeviceContext;
	ID3D11RenderTargetView *mp_BackBuffer;

public:
	RenderManager();
	~RenderManager();

	// Sets up and initializes window
	void InitWindow(HINSTANCE hInstance, int nCmdShow);
	// Sets up and initializes Direct3D
	void InitD3D(HWND hWnd);
	// Closes Direct3D and releases memory
	void CleanD3D(void);
	// Renders a frame
	void RenderFrame(void);		 
};

#endif

