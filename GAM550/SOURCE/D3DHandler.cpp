/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

D3DHandler::D3DHandler()
{
}

D3DHandler::~D3DHandler()
{
	CleanD3D();
}

HWND D3DHandler::InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings)
{
	// this struct holds information for the window class
	WNDCLASSEX wc;

	DEVMODE dmScreenSettings;
	int posX, posY;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = settings.WindowTitle.c_str();

	// register the window class
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (settings.FullScreen)
	{
		// Determine the resolution of the clients desktop screen.
		unsigned long screenWidth = GetSystemMetrics(SM_CXSCREEN);
		unsigned long screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = screenWidth;
		dmScreenSettings.dmPelsHeight = screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - settings.Width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - settings.Height) / 2;
	}


	const DWORD uStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	const DWORD uExStyle = WS_EX_APPWINDOW;

	RECT rect{ 0, 0, LONG(settings.Height), LONG(settings.Width) };
	AdjustWindowRectEx(&rect, uStyle, FALSE, uExStyle);

	// create the window and use the result as the handle
	HWND hWnd = CreateWindowEx(
		uExStyle,
		settings.WindowTitle.c_str(),	// name of the window class
		settings.WindowTitle.c_str(),   // title of the window
		uStyle,							// window style
		posX,							// x-position of the window
		posY,							// y-position of the window
		settings.Width,					// width of the window
		settings.Height,				// height of the window
		NULL,							// we have no parent window, NULL
		NULL,							// we aren't using menus, NULL
		hInstance,						// application handle
		NULL);							// used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	return hWnd;
}

bool D3DHandler::InitD3D(HWND hWnd, WindowSettings settings)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	size_t stringLength;
	unsigned int numModes, i, numerator, denominator;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc; // create a struct to hold information about the swap chain
	D3D_FEATURE_LEVEL featureLevel;
	//ID3D11Texture2D* backBufferPtr;
	//D3D11_TEXTURE2D_DESC depthBufferDesc;
	//D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	//D3D11_RASTERIZER_DESC rasterDesc;
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
	for (i = 0; i < numModes; i++)
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
	error = wcstombs_s(&stringLength, m_VideoCardDescription, 128, adapterDesc.Description, 128);
	
	if (error != 0)
		return false;

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
	swapChainDesc.BufferCount = 1;                                  // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // use 32-bit color
	swapChainDesc.BufferDesc.Width = settings.Width;		// set the back buffer width
	swapChainDesc.BufferDesc.Width = settings.Height;		// set the back buffer height
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how swap chain is to be used
	swapChainDesc.OutputWindow = hWnd;                              // the window to be used
	swapChainDesc.SampleDesc.Count = 4;                             // how many multisamples
	swapChainDesc.Windowed = !settings.FullScreen;                  // windowed/full-screen mode

	// Set the refresh rate of the back buffer.
	if (settings.VSync)
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


	mp_BackBuffer = new BackBufferRenderTarget();
	mp_BackBuffer->Initialize(settings, mp_Device, mp_DeviceContext);

#pragma region Viewport
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	// Setup the viewport for rendering.
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = FLOAT(settings.Width);
	viewport.Height = FLOAT(settings.Height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Create the viewport.
	mp_DeviceContext->RSSetViewports(1, &viewport);
#pragma endregion

	return true;
}

void D3DHandler::CleanD3D(void)
{
	// switch back to windowed mode, D3D will fail to close if it's still in fullscreen
	if (mp_SwapChain)
		mp_SwapChain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	if (mp_SwapChain)
		mp_SwapChain->Release();
	//if (mp_BackBuffer)
	//	mp_BackBuffer->Release();
	if (mp_Device)
		mp_Device->Release();
	if (mp_DeviceContext)
		mp_DeviceContext->Release();

	if (mp_BackBuffer)
		mp_BackBuffer->Release();

	//if (mp_DepthStencilBuffer)
	//	mp_DepthStencilBuffer->Release();
	//if (mp_DepthStencilState)
	//	mp_DepthStencilState->Release();
	//if (mp_DepthStencilView)
	//	mp_DepthStencilView->Release();
	//if (mp_RasterState)
	//	mp_RasterState->Release();
}

void D3DHandler::BindBackBuffer() const
{
	mp_BackBuffer->BindRenderTarget(mp_DeviceContext);
}

void D3DHandler::ClearBackBuffer(const Color& color)
{
	mp_BackBuffer->ClearRenderTarget(mp_DeviceContext, color);
}

void D3DHandler::PresentBuffer(bool vSync)
{
	// Swap the back buffer and the front buffer

	// Lock to screen refresh rate.
	if (vSync)
		mp_SwapChain->Present(1, 0);
	// Present as fast as possible.
	else
		mp_SwapChain->Present(0, 0);
}
