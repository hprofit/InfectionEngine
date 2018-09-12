#include "Stdafx.h"

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		INFECT_GAME_STATE.SetGameState(GameState::QUIT);
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool RenderManager::_GameObjectHasRenderableComponent(const GameObject & gameObject)
{
	return gameObject.HasComponent(ComponentType::C_Mesh);
}

RenderManager::RenderManager() :
	mp_SwapChain(nullptr),
	mp_Device(nullptr),
	mp_DeviceContext(nullptr),
	mp_BackBuffer(nullptr),
	mp_DepthBuffer(nullptr),
	m_ClearColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f)),
	m_ScreenWidth(800), m_ScreenHeight(600),
	m_FullScreen(false), m_VSyncEnabled(true)
{
}

RenderManager::~RenderManager()
{

}


void RenderManager::InitConsole()
{
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("Infect Engine");
	}
}

void RenderManager::DestroyConsole() 
{
	if (FreeConsole()) {
		
	}
}

void RenderManager::InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings)
{
	m_FullScreen = settings.FullScreen;
	m_ScreenWidth = settings.Width;
	m_ScreenHeight = settings.Height;
	m_AspectRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
	m_VSyncEnabled = settings.VSync;
	
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	m_hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		settings.WindowTitle.c_str(),   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		CW_USEDEFAULT,    // x-position of the window
		CW_USEDEFAULT,    // y-position of the window
		m_ScreenWidth,    // width of the window
		m_ScreenHeight,   // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(m_hWnd, nCmdShow);

	InitD3D(m_hWnd, settings);
}

bool RenderManager::InitD3D(HWND hWnd, WindowSettings settings) 
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc; // create a struct to hold information about the swap chain
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;

#pragma region Refresh Rate and Video Card Info
	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
		return false;

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
		return false;

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
		return false;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
		return false;

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
		return false;

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == settings.Width)
		{
			if (displayModeList[i].Height == settings.Height)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
		return false;

	// Store the dedicated video card memory in megabytes.
	m_VideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	//error = wcstombs_s(&stringLength, m_VideoCardDescription, 128, adapterDesc.Description, 128);
	//if (error != 0)
	//	return false;

	// Release the adapter output.
	adapterOutput->Release();

	// Release the adapter.
	adapter->Release();

	// Release the factory.
	factory->Release();
#pragma endregion
	
#pragma region Swap Chain, Device, Device Context Setup

	// clear out the struct for use
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	swapChainDesc.BufferCount = 1;                                    // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	swapChainDesc.BufferDesc.Width = m_ScreenWidth;					// set the back buffer width
	swapChainDesc.BufferDesc.Width = m_ScreenHeight;					// set the back buffer height
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	swapChainDesc.OutputWindow = hWnd;                                // the window to be used
	swapChainDesc.SampleDesc.Count = 4;                               // how many multisamples
	swapChainDesc.Windowed = settings.FullScreen;                     // windowed/full-screen mode

	// Set the refresh rate of the back buffer.
	if (m_VSyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;
	//swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// create a device, device context and swap chain using the information in the swapChainDesc struct
	result = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mp_SwapChain,
		&mp_Device,
		NULL,
		&mp_DeviceContext);
	if (FAILED(result))
		return false;

#pragma endregion

#pragma region BackBuffer
	// Set render target
	// get address of back buffer
	ID3D11Texture2D *pBackBuffer;
	result = mp_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(result))
		return false;
	mp_SwapChain->SetFullscreenState(m_FullScreen, NULL);

	// use the back buffer address to create the render target
	result = mp_Device->CreateRenderTargetView(pBackBuffer, NULL, &mp_BackBuffer);
	if (FAILED(result))
		return false;
	pBackBuffer->Release();
#pragma endregion

#pragma region DepthBuffer
	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = settings.Width;
	depthBufferDesc.Height = settings.Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = mp_Device->CreateTexture2D(&depthBufferDesc, NULL, &mp_DepthStencilBuffer);
	if (FAILED(result))
		return false;
#pragma endregion

#pragma region DepthStencil

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = mp_Device->CreateDepthStencilState(&depthStencilDesc, &mp_DepthStencilState);
	if (FAILED(result))
		return false;

	// Set the depth stencil state.
	mp_DeviceContext->OMSetDepthStencilState(mp_DepthStencilState, 1);
#pragma endregion

#pragma region DepthStencilView
	// Initailze the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = mp_Device->CreateDepthStencilView(mp_DepthStencilBuffer, &depthStencilViewDesc, &mp_DepthStencilView);
	if (FAILED(result))
		return false;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	mp_DeviceContext->OMSetRenderTargets(1, &mp_BackBuffer, 0);// mp_DepthStencilView);
#pragma endregion

#pragma region Rasterizer State
	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	//result = mp_Device->CreateRasterizerState(&rasterDesc, &mp_RasterState);
	//if (FAILED(result))
	//	return false;

	//// Now set the rasterizer state.
	//mp_DeviceContext->RSSetState(mp_RasterState);
#pragma endregion

#pragma region Viewport
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	// Setup the viewport for rendering.
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = FLOAT(m_ScreenWidth);
	viewport.Height = FLOAT(m_ScreenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Create the viewport.
	mp_DeviceContext->RSSetViewports(1, &viewport);
#pragma endregion

	return true;
}

// this is the function that cleans up Direct3D and COM
void RenderManager::CleanD3D()
{
	// switch back to windowed mode, D3D will fail to close if it's still in fullscreen
	if (mp_SwapChain)
		mp_SwapChain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	if (mp_VS)
		mp_VS->Release();
	if (mp_PS)
		mp_PS->Release();
	if (mp_Cbuffer)
		mp_Cbuffer->Release();

	if (mp_SwapChain)
		mp_SwapChain->Release();
	if (mp_BackBuffer)
		mp_BackBuffer->Release();
	if (mp_Device)
		mp_Device->Release();
	if (mp_DeviceContext)
		mp_DeviceContext->Release();

	if (mp_DepthStencilBuffer)
		mp_DepthStencilBuffer->Release();
	if (mp_DepthStencilState)
		mp_DepthStencilState->Release();
	if (mp_DepthStencilView)
		mp_DepthStencilView->Release();
	if (mp_RasterState)
		mp_RasterState->Release();
}

void RenderManager::FrameStart(void)
{
	mp_DeviceContext->ClearRenderTargetView(mp_BackBuffer, m_ClearColor);

	// Clear the depth buffer.
	mp_DeviceContext->ClearDepthStencilView(mp_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderManager::FrameEnd(void)
{
	// switch the back buffer and the front buffer

	// Present the back buffer to the screen since rendering is complete.
	if (m_VSyncEnabled)
	{
		// Lock to screen refresh rate.
		mp_SwapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		mp_SwapChain->Present(0, 0);
	}
}

void RenderManager::RenderObject(const GameObject& pGOCamera, const GameObject& pGO) 
{
	if (!_GameObjectHasRenderableComponent(pGO))
		return;

	const CameraComponent * pCamComp = pGOCamera.GetComponent<CameraComponent>();
	Matrix4x4 M = pGO.GetComponent<TransformComponent>()->GetTransform();
	Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));

	ConstantBuffer cb;
	cb.MatFinal = pCamComp->GetCameraMatrix() * pCamComp->GetViewMatrix() * M;
	cb.MatFinal.Transpose();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.NormalMatrix = Matrix4x4::Transpose(N);
	cb.CameraPosition = pGOCamera.GetComponent<TransformComponent>()->GetPosition();
	
	mp_DeviceContext->VSSetConstantBuffers(0, 1, &mp_Cbuffer);
	// set the new values for the constant buffer
	mp_DeviceContext->UpdateSubresource(mp_Cbuffer, 0, 0, &cb, 0, 0);

	// do 3D rendering on the back buffer here
	RenderScene(pGO.GetComponent<MeshComponent>()->GetScene());
}

void RenderManager::RenderScene(const Scene * pScene)
{
	for (int i = 0; i < pScene->NumMeshes(); ++i) {
		const Mesh* pMesh = (*pScene)[i];
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* buffers[] = { pMesh->VBuffer() };
		mp_DeviceContext->IASetVertexBuffers(0, 1, &(buffers[0]), &stride, &offset);
		mp_DeviceContext->IASetIndexBuffer(pMesh->IBuffer(), DXGI_FORMAT_R32_UINT, 0);

		mp_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//mp_DeviceContext->Draw(pMesh->NumVerts(), 0);
		mp_DeviceContext->DrawIndexed(pMesh->NumFaces()*3, 0, 0);
	}
}

void RenderManager::LoadShader()
{
	// load and compile the shaders
	int flag = D3D10_SHADER_WARNINGS_ARE_ERRORS;// | D3D10_SHADER_OPTIMIZATION_LEVEL3;
	D3DX11CompileFromFile("ASSETS/SHADERS/base3D.shader", 0, 0, "VShader", "vs_4_0", flag, 0, 0, &mp_VSBlob, &mp_Errors, 0);
	if (mp_Errors)
		MessageBox(NULL, "The vertex shader failed to compile.", "Error", MB_OK);

	D3DX11CompileFromFile("ASSETS/SHADERS/base3D.shader", 0, 0, "PShader", "ps_4_0", flag, 0, 0, &mp_PSBlob, &mp_Errors, 0);
	if (mp_Errors)
		MessageBox(NULL, "The pixel shader failed to compile.", "Error", MB_OK);

	// Encapsulate both shaders into shader objects
	mp_Device->CreateVertexShader(mp_VSBlob->GetBufferPointer(), mp_VSBlob->GetBufferSize(), NULL, &mp_VS);
	mp_Device->CreatePixelShader(mp_PSBlob->GetBufferPointer(), mp_PSBlob->GetBufferSize(), NULL, &mp_PS);

	mp_DeviceContext->VSSetShader(mp_VS, 0, 0);
	mp_DeviceContext->PSSetShader(mp_PS, 0, 0);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = 208;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	mp_Device->CreateBuffer(&bd, NULL, &mp_Cbuffer);
}
