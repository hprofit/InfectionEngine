/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

//struct ConstantBuffer {
//	Matrix4x4 MatFinal;
//	Matrix4x4 ModelMatrix;
//	Matrix4x4 NormalMatrix;
//	Vector3D CameraPosition;
//	Vector3D LightPosition;
//	int CastShadows;
//	int ReceiveShadows;
//	int IsLit;
//	int Textured;
//};

class RenderManager : public Subscriber
{
private:
	D3DHandler *mp_D3D;

	Color m_ClearColor;
	WindowSettings m_WindowSettings;
	HWND m_hWnd; 	// the handle for the window, filled by a function

	unsigned int mShaderCount = 0;
	ShaderProgram* mp_ShaderProgramDefault;
	ShaderProgram* mp_ShaderProgramDeferred;
	ShaderProgram* mp_ShaderProgramQuad;

	bool _GameObjectHasRenderableComponent(const GameObject& gameObject);
public:
	RenderManager();
	~RenderManager();

	inline IDXGISwapChain * const SwapChain() { return mp_D3D->SwapChain(); }
	inline ID3D11Device * const Device() { return mp_D3D->Device(); }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_D3D->DeviceContext(); }

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


	void BindBackBuffer();

	void BindDeferredBuffer();

	void RenderDeferredBuffer();

	void ClearScreen(void);

	void PresentFrameToScreen(void);

	// Renders an object given a specific camera
	void RenderObject(const GameObject& pGOCamera, const GameObject& pGO);

	void RenderScene(const Scene * pScene);

	bool LoadShader(std::string shaderName);

	ID3D10Blob * const VSBlob() { return mp_ShaderProgramDefault->VS()->Blob(); }
};

#endif

