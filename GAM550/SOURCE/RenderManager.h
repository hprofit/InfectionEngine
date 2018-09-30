/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once

#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

enum RenderMode {
	WorldPos = 0,
	Normal,
	Diffuse,
	Specular,
	Final,

	NUM_MODES
};

class RenderManager : public Subscriber
{
private:
	D3DHandler *mp_D3D;

	Color m_ClearColor;
	WindowSettings m_WindowSettings;
	HWND m_hWnd; 	// the handle for the window, filled by a function

	unsigned int mShaderCount = 0;
	//ShaderProgram* mp_ShaderProgramDefault;
	ShaderProgram<MainCB>* mp_ShaderProgramDeferred;
	ShaderProgram<QuadCB>* mp_ShaderProgramQuad;
	ShaderProgram<DeferredFinalCB>* mp_ShaderProgramDeferredFinal;

	RenderMode m_RenderMode;

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

	void PrepDeferredPass();

	void BindSecondPassBuffer();

	void PrepDeferredFinal();

	// For Debug only
	void RenderDeferredBuffer();

	void RenderDeferredBufferAmbientOnly();

	void RenderSecondPassBuffer();

	void ClearScreen(void);

	void PresentFrameToScreen(void);

	// Renders an object given a specific camera
	void RenderObject(const GameObject& pGOCamera, const GameObject& pGO);

	// Renders a light given a specific camera
	void RenderLight(const GameObject& pGOCamera, const GameObject& pGOLight);

	void RenderScene(const Scene * pScene);

	bool LoadShader(std::string shaderName);

	//ID3D10Blob * const VSBlob() { return mp_ShaderProgramDefault->VS()->Blob(); }
	ID3D10Blob * const VSBlob() { return mp_ShaderProgramDeferred->VS()->Blob(); }

	// TODO: Remove this
	void NextRenderMode();

	inline RenderMode CurrentRenderMode() const { return m_RenderMode; }
};

#endif

