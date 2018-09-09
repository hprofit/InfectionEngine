#include <Stdafx.h>


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
	INFECT_EVENTS.Init();

	// Main Loop
	// TODO: Move this

	// this struct holds Windows event messages
	MSG msg;
	//INFECT_RENDERER.InitWindow(hInstance, nCmdShow, true, 1920, 1080);
	INFECT_RENDERER.InitWindow(hInstance, nCmdShow, false, 800, 600);
	INFECT_RENDERER.LoadShader();


	Mesh* pMesh = new Mesh();
	pMesh->AddVertex(0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pMesh->AddVertex(0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pMesh->AddVertex(-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pMesh->FinishMesh();
	Scene* pScene = new Scene(1);
	(*pScene)[0] = pMesh;



	GameObject* pGO = INFECT_GOM.GetGameObject(0);
	MeshComponent* pMeshComp = new MeshComponent();
	pMeshComp->SetScene(INFECT_RESOURCES.GetScene("Suzy.fbx"));
	
	TransformComponent* pTransComp = new TransformComponent();
	pTransComp->SetPosition(Vector3D(0, 0, 0, 1));

	pGO->AddComponent(pMeshComp);
	pGO->AddComponent(pTransComp);



	GameObject* pGOCamera = INFECT_GOM.GetGameObject(0);
	pTransComp = new TransformComponent();
	pTransComp->SetPosition(Vector3D(0, 0, 10, 1));

	CameraComponent * pCamComp = new CameraComponent();
	pGOCamera->AddComponent(pTransComp);
	pGOCamera->AddComponent(pCamComp);
	pCamComp->LateInitialize();
	pCamComp->LateUpdate(0);

	// wait for the next message in the queue, store the result in 'msg'
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);

		INFECT_RENDERER.RenderFrame(pGOCamera, pGO);
	}
	INFECT_RENDERER.CleanD3D();
	// return this part of the WM_QUIT message to Windows
	return (int)msg.wParam;
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
