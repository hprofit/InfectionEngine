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
	Depth,
	Light,
	Final,

	NUM_MODES
};

class RenderManager : public Subscriber
{
private:
	D3DHandler *mp_D3D;

	Color m_ClearColor;					// Color to clear buffers with
	Color m_Ambient;					// Ambient color to render with - Temporary, this will be replaced
	WindowSettings m_WindowSettings;	// Settings of the window contained for easy reference
	HWND m_hWnd; 						// Handle for the window

	unsigned int mShaderCount = 0;		// Horrible, lame way of knowing which shader we've loaded already, REPLACE THIS
	//ShaderProgram* mp_ShaderProgramDefault;	// Forward rendering shader, no longer used
	ShaderProgram<MainCB>* mp_ShaderProgramDeferred;					// First pass deferred shader
	ShaderProgram<QuadCB>* mp_ShaderProgramQuad;						// Quad rendering shader
	ShaderProgram<DeferredFinalCB>* mp_ShaderProgramDeferredFinal;		// Second pass deferred shader
	ShaderProgram<ShadowCB>* mp_ShaderProgramShadowCastingLight;		// Shadow casting light shader
	ShaderProgram<ShadowAddLightCB>* mp_ShaderProgramShadowAddLight;	// Shadow add light shader - determines wither to add a given light's influence to a pixel based on that light's shadowmap
	ShaderProgram<MainCB>* mp_ShaderProgramParticles;					// 

	RenderMode m_RenderMode;			// DEBUG ONLY - which layer of the deferred buffer to render

	bool _GameObjectHasRenderableComponent(const GameObject& gameObject);
	void _RenderScene(const Scene * pScene);
public:
	RenderManager();
	~RenderManager();

	inline IDXGISwapChain * const SwapChain() { return mp_D3D->SwapChain(); }
	inline ID3D11Device * const Device() { return mp_D3D->Device(); }
	inline ID3D11DeviceContext * const DeviceContext() { return mp_D3D->DeviceContext(); }

	inline FLOAT AspectRatio() const { return m_WindowSettings.AspectRatio; }
	inline UINT ScreenWidth() const { return m_WindowSettings.Width; }
	inline UINT ScreenHeight() const { return m_WindowSettings.Height; }
	inline HWND GethWnd() { return m_hWnd; }

	// Creates a console for output
	void InitConsole();
	// Destroys the active console
	void DestroyConsole();
	// Sets up and initializes window
	bool InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings);

	// Binds the D3DHandler's back buffer for drawing
	void BindBackBuffer();

	// Enables back face culling, binds the D3DHandler's deferred buffer for drawing and binds
	// the deferred first pass shader, enables depth testing and disables alpha blending
	void PrepDeferredPass();

	// Binds the D3DHandler's secondary buffer for drawing
	void BindSecondPassBuffer();

	// Binds the deferred second pass shader, sets the D3DHandler's deferred buffer as the active textures,
	// disables depth testing and enables alpha blending
	void PrepDeferredFinal();

	// For Debug only
	// Renders the individual textures within the D3DHandler's deferred buffer (i.e. the G-Buffer) according to the
	// current RenderMode
	// THIS SHOULD NOT BE USED IN FINAL
	void RenderDebugBuffers();

	// Binds the quad shader, disables depth testing, disables alpha blending
	// Ambient light is set to RenderManager's current m_Ambient value
	// The D3DHandler's deferred buffer's diffuse layer is bound as the texture and rendered 
	void RenderDeferredBufferAmbientOnly();

	// Binds the quad shader, disables depth testing, disables alpha blending
	// Ambient light is set to RenderManager's current m_Ambient value
	// The D3DHandler's secondary buffer texture is bound as the active texture and rendered 
	void RenderSecondPassBuffer();

	// Enables front face culling, binds the shadow casting light shader, enables depth testing, disables alpha blending
	void PrepShadowCastingLightPass();

	// Binds the shadow shader, sets the D3DHandler's deferred buffer as the active textures, 
	// disables depth testing, and enables alpha blending
	void PrepShadowCastingLightFinal();

	// Clears the back buffer (screen) as well as any other built in buffers the D3DHandler has
	void ClearScreen(void);

	// Writes the contents of the back buffer to the screen
	void PresentFrameToScreen(void);

	// Renders an object given a specific camera
	void RenderObject(const GameObject& pGOCamera, const GameObject& pGO);

	// Renders a light given a specific camera
	void RenderLight(const GameObject& pGOCamera, const GameObject& pGOLight);

	// Given a shadow casting light and a game object, renders the game object
	// to the shadow casting light's shadow map/depth buffer
	void RenderObjectToLightShadowMap(const GameObject& goLight, const GameObject& go);

	// Given a shadow casting light, re-renders the scene adding the light's
	// influence to any affected pixels (post deferred G-Buffer rendering, pre deferred-shading)
	void AddSCLInfluenceToScene(const GameObject & pGOCamera, GameObject& goLight);

	bool LoadShader(std::string shaderName);

	//ID3D10Blob * const VSBlob() { return mp_ShaderProgramDefault->VS()->Blob(); }
	ID3D10Blob * const VSBlob() { return mp_ShaderProgramDeferred->VS()->Blob(); }

	// TODO: Remove this
	void NextRenderMode();
	// TODO: Remove this
	inline RenderMode CurrentRenderMode() const { return m_RenderMode; }


	void PrepParticlePass();
	// TODO: GET RID OF THIS
	void RenderParticles(const GameObject& pGOCamera, const GameObject& pGO);
};

#endif

