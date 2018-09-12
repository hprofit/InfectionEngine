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
}


void D3DHandler::InitConsole()
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

void D3DHandler::DestroyConsole()
{
	if (FreeConsole()) {

	}
}

void D3DHandler::InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings)
{
	// the handle for the window, filled by a function
	HWND hWnd;
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
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		settings.WindowTitle.c_str(),   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		CW_USEDEFAULT,    // x-position of the window
		CW_USEDEFAULT,    // y-position of the window
		settings.Width,    // width of the window
		settings.Height,   // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	InitD3D(hWnd, settings);
}

void D3DHandler::InitD3D(HWND hWnd, WindowSettings settings) {
	// Direct3D initialization
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = settings.Width;					// set the back buffer width
	scd.BufferDesc.Width = settings.Height;					// set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
															//scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

															// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&mp_SwapChain,
		&mp_Device,
		NULL,
		&mp_DeviceContext);


	// Set render target
	// get address of back buffer
	ID3D11Texture2D *pBackBuffer;
	mp_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	mp_SwapChain->SetFullscreenState(settings.FullScreen, NULL);

	// use the back buffer address to create the render target
	mp_Device->CreateRenderTargetView(pBackBuffer, NULL, &mp_BackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	mp_DeviceContext->OMSetRenderTargets(1, &mp_BackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = FLOAT(settings.Width);
	viewport.Height = FLOAT(settings.Height);

	mp_DeviceContext->RSSetViewports(1, &viewport);
}

// this is the function that cleans up Direct3D and COM
void D3DHandler::CleanD3D()
{
	// switch back to windowed mode, D3D will fail to close if it's still in fullscreen
	mp_SwapChain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	//mp_VS->Release();
	//mp_PS->Release();
	//mp_Cbuffer->Release();

	mp_SwapChain->Release();
	mp_BackBuffer->Release();
	mp_Device->Release();
	mp_DeviceContext->Release();
}
