#include "Stdafx.h"

// Temporarily putting this here - I believe it's a better fit for InputManager
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

RenderManager::RenderManager() : 
	mp_SwapChain(nullptr),
	mp_Device(nullptr),
	mp_DeviceContext(nullptr),
	mp_BackBuffer(nullptr),
	m_ClearColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f)),
	m_ScreenWidth(800), m_ScreenHeight(600),
	m_FullScreen(false)
{
}

RenderManager::~RenderManager()
{
}


void RenderManager::InitWindow(HINSTANCE hInstance, int nCmdShow, bool fullScreen, unsigned int screenWidth, unsigned int screenHeight)
{
	m_FullScreen = fullScreen;
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;
	m_AspectRatio = (float)m_ScreenWidth / (float)m_ScreenHeight;
	
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
		"Our First Windowed Program",   // title of the window
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
	scd.BufferDesc.Width = m_ScreenWidth;					// set the back buffer width
	scd.BufferDesc.Width = m_ScreenHeight;					// set the back buffer height
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
	mp_SwapChain->SetFullscreenState(m_FullScreen, NULL);

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
	viewport.Width = FLOAT(m_ScreenWidth);
	viewport.Height = FLOAT(m_ScreenHeight);

	mp_DeviceContext->RSSetViewports(1, &viewport);
}

// this is the function that cleans up Direct3D and COM
void RenderManager::CleanD3D()
{
	// switch back to windowed mode, D3D will fail to close if it's still in fullscreen
	mp_SwapChain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	mp_VS->Release();
	mp_PS->Release();

	mp_SwapChain->Release();
	mp_BackBuffer->Release();
	mp_Device->Release();
	mp_DeviceContext->Release();
}

void RenderManager::RenderFrame(const GameObject* pGOCamera, const GameObject* pGO) {
	mp_DeviceContext->ClearRenderTargetView(mp_BackBuffer, m_ClearColor);

	// do 3D rendering on the back buffer here
	RenderScene(pGO->GetComponent<MeshComponent>(C_Mesh)->GetScene());

	// switch the back buffer and the front buffer
	mp_SwapChain->Present(0, 0);
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
}

void RenderManager::RenderScene(const Scene * pScene)
{
	for (int i = 0; i < pScene->NumMeshes(); ++i) {
		const Mesh* pMesh = (*pScene)[i];
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* buffers[] = { pMesh->VBuffer() };
		mp_DeviceContext->IASetVertexBuffers(0, 1, &(buffers[0]), &stride, &offset);

		mp_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mp_DeviceContext->Draw(3, 0);
	}
}
