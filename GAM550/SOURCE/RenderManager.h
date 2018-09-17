/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

struct ConstantBuffer {
	Matrix4x4 MatFinal;
	Matrix4x4 ModelMatrix;
	Matrix4x4 NormalMatrix;
	bool CastShadows;
	bool ReceiveShadows;
	bool IsLit;
	Vector3D CameraPosition;
	Vector3D LightPosition;
};

class RenderManager : public Subscriber
{
private:
	D3DHandler *mp_D3D;

	D3DXCOLOR m_ClearColor;
	WindowSettings m_WindowSettings;
	HWND m_hWnd; 	// the handle for the window, filled by a function

	ID3D11Buffer *mp_Cbuffer;		// the constant buffer
	ID3D11VertexShader *mp_VS;		// the vertex shader
	ID3D11PixelShader *mp_PS;		// the pixel shader
	ID3D10Blob *mp_VSBlob, *mp_PSBlob, *mp_Errors;

	bool _GameObjectHasRenderableComponent(const GameObject& gameObject);
public:
	RenderManager();
	~RenderManager();

	inline IDXGISwapChain * const SwapChain() { return mp_D3D->SwapChain(); }
	inline ID3D11Device * const Device() { return mp_D3D->Device(); }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_D3D->DeviceContext(); }
	inline ID3D11RenderTargetView * const BackBuffer() { return mp_D3D->BackBuffer(); }
	inline ID3D11RenderTargetView * const DepthBuffer() { return mp_D3D->BackBuffer(); }

	inline float AspectRatio() const { return m_WindowSettings.AspectRatio; }
	inline unsigned int ScreenWidth() const { return m_WindowSettings.Width; }
	inline unsigned int ScreenHeight() const { return m_WindowSettings.Height; }
	inline HWND GethWnd() { return m_hWnd; }

	// Creates a console for output
	void InitConsole();
	// Destroys the active console
	void DestroyConsole();
	// Sets up and initializes window
	bool InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings);


	void FrameStart(void);

	void FrameEnd(void);

	// Renders an object given a specific camera
	void RenderObject(const GameObject& pGOCamera, const GameObject& pGO);

	void RenderScene(const Scene * pScene);

	bool LoadShader(void);

	// TODO: Get rid of these in favor of selecting which VS/PS you want
	ID3D11VertexShader * const VertexShader() { return mp_VS; }
	ID3D11PixelShader * const PixelShader() { return mp_PS; }
	ID3D10Blob * const VSBlob() { return mp_VSBlob; }
	ID3D10Blob * const PSBlob() { return mp_PSBlob; }
};

#endif

