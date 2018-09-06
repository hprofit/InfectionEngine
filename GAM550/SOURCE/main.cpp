#include <Stdafx.h>

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

IDXGISwapChain *P_SWAP_CHAIN;
ID3D11Device *P_DEVICE;
ID3D11DeviceContext *P_DEVICE_CONTEXT;
ID3D11RenderTargetView *P_BACKBUFFER;

// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CleanD3D(void);         // closes Direct3D and releases memory

std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

int Mode() {
	std::string path = ExePath();
	if (path.find("Debug") || path.find("Release"))
		return 0;
	else
		return 1;
}

/*
int nCmdShow

Value	Description
SW_SHOWNORMAL	Activates and displays a window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time.
SW_SHOW	Activates the window and displays it in its current size and position.
SW_HIDE	Hides the window and activates another window.
SW_MAXIMIZE	Maximizes the specified window.
SW_MINIMIZE	Minimizes the specified window and activates the next top-level window in the Z order.
SW_RESTORE	Activates and displays the window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when restoring a minimized window.
SW_SHOWMAXIMIZED	Activates the window and displays it as a maximized window.
SW_SHOWMINIMIZED	Activates the window and displays it as a minimized window.
SW_SHOWMINNOACTIVE	Displays the window as a minimized window. This value is similar to SW_SHOWMINIMIZED, except the window is not activated.
SW_SHOWNA	Displays the window in its current size and position. This value is similar to SW_SHOW, except the window is not activated.
SW_SHOWNOACTIVATE	Displays a window in its most recent size and position. This value is similar to SW_SHOWNORMAL, except the window is not actived.
*/
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
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

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;
	InitD3D(hWnd);


	// wait for the next message in the queue, store the result in 'msg'
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}
	CleanD3D();
	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitD3D(HWND hWnd) {
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
		&P_SWAP_CHAIN,
		&P_DEVICE,
		NULL,
		&P_DEVICE_CONTEXT);

	// get address of back buffer
	ID3D11Texture2D *pBackBuffer;
	P_SWAP_CHAIN->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	P_DEVICE->CreateRenderTargetView(pBackBuffer, NULL, &P_BACKBUFFER);
	pBackBuffer->Release();

	// set the render target as the back buffer
	P_DEVICE_CONTEXT->OMSetRenderTargets(1, &P_BACKBUFFER, NULL);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D()
{
	// close and release all existing COM objects
	P_SWAP_CHAIN->Release();
	P_DEVICE->Release();
	P_DEVICE_CONTEXT->Release();
}