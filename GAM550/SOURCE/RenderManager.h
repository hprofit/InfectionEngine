/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

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
	D3DXCOLOR m_ClearColor;
	unsigned int m_ScreenWidth, m_ScreenHeight;
	bool m_FullScreen;



	ID3D11VertexShader *mp_VS;    // the vertex shader
	ID3D11PixelShader *mp_PS;     // the pixel shader
	ID3D10Blob *mp_VSBlob, *mp_PSBlob;

public:
	RenderManager();
	~RenderManager();

	inline IDXGISwapChain * const SwapChain() { return mp_SwapChain; }
	inline ID3D11Device * const Device() { return mp_Device; }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_DeviceContext; }
	inline ID3D11RenderTargetView * const BackBuffer() { return mp_BackBuffer; }


	// Sets up and initializes window
	void InitWindow(HINSTANCE hInstance, int nCmdShow, bool fullScreen, unsigned int screenWidth, unsigned int screenHeight);
	// Sets up and initializes Direct3D
	void InitD3D(HWND hWnd);
	// Closes Direct3D and releases memory
	void CleanD3D(void);
	// Renders a frame
	void RenderFrame(Mesh* const pMesh);	

	void LoadShader();

	void RenderMesh(Mesh* const pMesh);



	// TODO: Get rid of these in favor of selecting which VS/PS you want
	ID3D11VertexShader * const VertexShader() { return mp_VS; }
	ID3D11PixelShader * const PixelShader() { return mp_PS; }
	ID3D10Blob * const VSBlob() { return mp_VSBlob; }
	ID3D10Blob * const PSBlob() { return mp_PSBlob; }
};

#endif

