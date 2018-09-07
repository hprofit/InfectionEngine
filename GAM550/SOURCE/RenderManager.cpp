#include "Stdafx.h"
#include "RenderManager.h"

// Temporarily putting this here - I believe it's a better fit for InputManager
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

RenderManager::RenderManager()
{
}


RenderManager::~RenderManager()
{
}


void RenderManager::InitWindow(HINSTANCE hInstance, int nCmdShow)
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
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		"Our First Windowed Program",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		CW_USEDEFAULT,    // x-position of the window
		CW_USEDEFAULT,    // y-position of the window
		800,    // width of the window
		600,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	InitD3D(hWnd);
}

void RenderManager::InitD3D(HWND hWnd) {
	// Direct3D initialization
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

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
	viewport.Width = 800;
	viewport.Height = 600;

	mp_DeviceContext->RSSetViewports(1, &viewport);
}

// this is the function that cleans up Direct3D and COM
void RenderManager::CleanD3D()
{
	// close and release all existing COM objects
	mp_SwapChain->Release();
	mp_BackBuffer->Release();
	mp_Device->Release();
	mp_DeviceContext->Release();
}

void RenderManager::RenderFrame(void) {
	mp_DeviceContext->ClearRenderTargetView(mp_BackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	mp_SwapChain->Present(0, 0);
}