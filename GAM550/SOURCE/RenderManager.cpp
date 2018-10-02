/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

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
		INFECT_THREAD_JOBS.AddNewJob(new RenderTerminateTerminate(*INFECT_THREAD_JOBS.GetThreadContainer<RenderThreadContainer>(ThreadType::RenderThread)));
		INFECT_THREAD_JOBS.AddNewJob(new SimulationTerminateTerminate(*INFECT_THREAD_JOBS.GetThreadContainer<SimulationThreadContainer>(ThreadType::SimThread)));
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
	m_ClearColor(Color(0.0f, 0.0f, 0.0f, 1)),
	mp_D3D(new D3DHandler()),
	m_RenderMode(RenderMode::WorldPos)
{
}

RenderManager::~RenderManager()
{
	if (mp_ShaderProgramDeferred) {
		mp_ShaderProgramDeferred->Release();
		delete mp_ShaderProgramDeferred;
		mp_ShaderProgramDeferred = nullptr;
	}
	if (mp_ShaderProgramQuad) {
		mp_ShaderProgramQuad->Release();
		delete mp_ShaderProgramQuad;
		mp_ShaderProgramQuad = nullptr;
	}
	if (mp_ShaderProgramDeferredFinal) {
		mp_ShaderProgramDeferredFinal->Release();
		delete mp_ShaderProgramDeferredFinal;
		mp_ShaderProgramDeferredFinal = nullptr;
	}

	// Show the mouse cursor.
	ShowCursor(true);
	delete mp_D3D;
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

bool RenderManager::InitWindow(HINSTANCE hInstance, int nCmdShow, WindowSettings settings)
{
	m_WindowSettings = settings;
	m_hWnd = mp_D3D->InitWindow(hInstance, nCmdShow, settings);
	bool result = mp_D3D->InitD3D(m_hWnd, settings);

	if (result)
	{
		mp_ShaderProgramDeferred = new ShaderProgram<MainCB>();
		mp_ShaderProgramQuad = new ShaderProgram<QuadCB>();
		mp_ShaderProgramDeferredFinal = new ShaderProgram<DeferredFinalCB>();
	}
	return result;
}

void RenderManager::BindBackBuffer()
{
	mp_D3D->BindBackBuffer();
}

void RenderManager::PrepDeferredPass()
{
	mp_D3D->BindDeferredBuffer();
	mp_ShaderProgramDeferred->BindShader();

	mp_D3D->EnableDepth();
	mp_D3D->DisableAlpha();
}

void RenderManager::BindSecondPassBuffer()
{
	mp_D3D->BindSecondPassBuffer();
}

void RenderManager::PrepDeferredFinal()
{
	mp_ShaderProgramDeferredFinal->BindShader();
	ID3D11ShaderResourceView** pTextures = mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews();
	mp_D3D->mp_DeviceContext->PSSetShaderResources(0, mp_D3D->GetDeferredRenderTarget()->GetNumViews(), pTextures);
	
	mp_D3D->DisableDepth();
	mp_D3D->EnableAlpha();
}

// For Debug only
void RenderManager::RenderDeferredBuffer()
{
	mp_D3D->BindBackBuffer();
	mp_ShaderProgramQuad->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();
	Matrix4x4 M = Matrix4x4::Scale(2, 2, 1);
	QuadCB& cb = mp_ShaderProgramQuad->CB()->BufferData();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.Ambient = Color(1, 1, 1, 1);
	mp_ShaderProgramQuad->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
	
	switch (m_RenderMode) {
		case RenderMode::Final:
		{
			mp_D3D->mp_DeviceContext->PSSetShaderResources(
				0,
				mp_D3D->GetDeferredRenderTarget()->GetNumViews(),
				mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews()
			);
			break;
		}
		default:
		{
			mp_D3D->mp_DeviceContext->PSSetShaderResources(
				0,
				1,
				&mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews()[m_RenderMode]
			);
			break;
		}
	}

	// set the new values for the constant buffer
	mp_ShaderProgramQuad->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	// do 3D rendering on the back buffer here
	RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
}

void RenderManager::RenderDeferredBufferAmbientOnly()
{
	mp_ShaderProgramQuad->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();

	Matrix4x4 M = Matrix4x4::Scale(2, 2, 1);
	QuadCB& cb = mp_ShaderProgramQuad->CB()->BufferData();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.Ambient = Color(0.6f, 0.6f, 0.6f, 1);
	mp_ShaderProgramQuad->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramQuad->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	mp_D3D->mp_DeviceContext->PSSetShaderResources(
		0,
		1,
		&mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews()[RenderMode::Diffuse]
	);

	// do 3D rendering on the back buffer here
	RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
}

void RenderManager::RenderSecondPassBuffer()
{
	mp_ShaderProgramQuad->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();

	Matrix4x4 M = Matrix4x4::Scale(2, 2, 1);
	QuadCB& cb = mp_ShaderProgramQuad->CB()->BufferData();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.Ambient = Color(0.6f, 0.6f, 0.6f, 1);
	mp_ShaderProgramQuad->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramQuad->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	mp_D3D->mp_DeviceContext->PSSetShaderResources(
		0,
		1,
		&mp_D3D->GetSecondPassRenderTarget()->GetShaderResourceViews()[0]
	);

	// do 3D rendering on the back buffer here
	RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
}

void RenderManager::ClearScreen(void)
{
	mp_D3D->ClearBackBuffer(m_ClearColor);
	mp_D3D->ClearDeferredBuffer(m_ClearColor);
	mp_D3D->ClearSecondPassBuffer(m_ClearColor);
}

void RenderManager::PresentFrameToScreen(void)
{
	mp_D3D->PresentBuffer(m_WindowSettings.VSync);
}

void RenderManager::RenderObject(const GameObject& pGOCamera, const GameObject& pGO) 
{
	if (!_GameObjectHasRenderableComponent(pGO))
		return;

	const CameraComponent * pCamComp = pGOCamera.GetComponent<CameraComponent>();
	const MeshComponent * pMeshComp = pGO.GetComponent<MeshComponent>();

	Matrix4x4 M = pGO.GetComponent<TransformComponent>()->GetTransform();
	Matrix4x4 N = Matrix4x4::Transpose3x3(Matrix4x4::Inverse3x3(M));
	MainCB& cb = mp_ShaderProgramDeferred->CB()->BufferData();
	cb.MatFinal = pCamComp->GetCameraMatrix() * pCamComp->GetViewMatrix() * M;
	cb.MatFinal.Transpose();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.NormalMatrix = Matrix4x4::Transpose(N);
	cb.CastShadows = pMeshComp->CastShadows();
	cb.ReceiveShadows = pMeshComp->ReceiveShadows();
	cb.IsLit = pMeshComp->IsLit();
	cb.TextureFlags = pMeshComp->TexturedFlags();
	cb.CameraPosition = pGOCamera.GetComponent<TransformComponent>()->WorldPosition();

	mp_ShaderProgramDeferred->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
	// set the new values for the constant buffer
	mp_ShaderProgramDeferred->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	//ID3D11ShaderResourceView* ptex = pMeshComp->GetDiffuseTexture();
	//mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &ptex);
	mp_D3D->mp_DeviceContext->PSSetShaderResources(0,
		TextureType::NUM_TEXTURE_TYPES,
		pMeshComp->GetTextures()
	);

	// do 3D rendering to the currently bound buffer here
	RenderScene(pMeshComp->GetScene());
}

void RenderManager::RenderLight(const GameObject & pGOCamera, const GameObject & pGOLight)
{
	const CameraComponent * pCamComp = pGOCamera.GetComponent<CameraComponent>();
	const TransformComponent * pTransComp = pGOLight.GetComponent<TransformComponent>();
	const PointLightComponent * pPointLightComp = pGOLight.GetComponent<PointLightComponent>();
	Matrix4x4 M = pTransComp->GetTransform();
	DeferredFinalCB& cb = mp_ShaderProgramDeferredFinal->CB()->BufferData();

	cb.MatFinal = pCamComp->GetCameraMatrix() * pCamComp->GetViewMatrix() * M;
	cb.MatFinal.Transpose();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.CameraPosition = pGOCamera.GetComponent<TransformComponent>()->WorldPosition();
	cb.LightPosition = pTransComp->WorldPosition();
	cb.LightPosition.w = pPointLightComp->LightA();
	cb.LightColor = pPointLightComp->GetColor();
	cb.LightColor.a = pPointLightComp->LightB();
	cb.LIDHW.x = pPointLightComp->Intensity();
	cb.LIDHW.y = pPointLightComp->Distance();
	cb.LIDHW.z = float(m_WindowSettings.Height);
	cb.LIDHW.w = float(m_WindowSettings.Width);
	cb.Ambient = Color(0.4f, 0.4f, 0.4f, 1);

	mp_ShaderProgramDeferredFinal->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramDeferredFinal->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	// do 3D rendering to the currently bound buffer here
	RenderScene(pPointLightComp->GetScene());
}

void RenderManager::RenderScene(const Scene * pScene)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (int i = 0; i < pScene->NumMeshes(); ++i) {
		const Mesh* pMesh = (*pScene)[i];
		ID3D11Buffer* buffers[] = { pMesh->VBuffer() };
		mp_D3D->mp_DeviceContext->IASetVertexBuffers(0, 1, &(buffers[0]), &stride, &offset);
		mp_D3D->mp_DeviceContext->IASetIndexBuffer(pMesh->IBuffer(), DXGI_FORMAT_R32_UINT, 0);

		mp_D3D->mp_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mp_D3D->mp_DeviceContext->DrawIndexed(pMesh->NumFaces()*3, 0, 0);
	}
}

bool RenderManager::LoadShader(std::string shaderName)
{
	std::string filePath = INFECT_GAME_CONFIG.ShadersDir() + shaderName;

	// TODO: Made this less shitty
	switch (mShaderCount) {
		case 0:
			mp_ShaderProgramDeferred->Initialize(mp_D3D->mp_Device, filePath);
			break;
		case 1:
			mp_ShaderProgramQuad->Initialize(mp_D3D->mp_Device, filePath);
			break;
		case 2:
			mp_ShaderProgramDeferredFinal->Initialize(mp_D3D->mp_Device, filePath);
		default:
			break;
	}
	++mShaderCount;

	return true;
}

void RenderManager::NextRenderMode()
{
	m_RenderMode = RenderMode(int(m_RenderMode) + 1);
	if (m_RenderMode >= RenderMode::NUM_MODES)
		m_RenderMode = RenderMode(0);
}
