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

void RenderManager::_RenderScene(const Scene * pScene)
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

RenderManager::RenderManager() :
	m_ClearColor(Color(0.0f, 0.0f, 0.0f, 1)),
	m_Ambient(Color(0.075f, 0.075f, 0.075f, 1)),
	mp_D3D(new D3DHandler()),
	m_RenderMode(RenderMode::Final)
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
	if (mp_ShaderProgramShadowCastingLight) {
		mp_ShaderProgramShadowCastingLight->Release();
		delete mp_ShaderProgramShadowCastingLight;
		mp_ShaderProgramShadowCastingLight = nullptr;
	}
	if (mp_ShaderProgramShadowAddLight) {
		mp_ShaderProgramShadowAddLight->Release();
		delete mp_ShaderProgramShadowAddLight;
		mp_ShaderProgramShadowAddLight = nullptr;
	}
	if (mp_ShaderProgramGaussianBlur) {
		mp_ShaderProgramGaussianBlur->Release();
		delete mp_ShaderProgramGaussianBlur;
		mp_ShaderProgramGaussianBlur = nullptr;
	}
    if (mp_ShaderProgramShadowMapHBlur) {
        mp_ShaderProgramShadowMapHBlur->Release();
        delete mp_ShaderProgramShadowMapHBlur;
        mp_ShaderProgramShadowMapHBlur = nullptr;
    }
    if (mp_ShaderProgramShadowMapVBlur) {
        mp_ShaderProgramShadowMapVBlur->Release();
        delete mp_ShaderProgramShadowMapVBlur;
        mp_ShaderProgramShadowMapVBlur = nullptr;
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
		mp_ShaderProgramShadowCastingLight = new ShaderProgram<ShadowCB>();
		mp_ShaderProgramShadowAddLight = new ShaderProgram<ShadowAddLightCB>();
		mp_ShaderProgramGaussianBlur = new ShaderProgram<BlurCB>();
        mp_ShaderProgramShadowMapHBlur = new ShaderProgram<Blur2CB>();
        mp_ShaderProgramShadowMapVBlur = new ShaderProgram<Blur2CB>();
	}
	return result;
}

void RenderManager::BindBackBuffer()
{
	mp_D3D->BindBackBuffer();
}

void RenderManager::PrepDeferredPass()
{
	mp_D3D->EnableBackFaceCulling();
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
void RenderManager::RenderDebugBuffers()
{
	mp_ShaderProgramQuad->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();
	QuadCB& cb = mp_ShaderProgramQuad->CB()->BufferData();
	cb.ModelMatrix = Matrix4x4::Scale(2, 2, 1);
	cb.Ambient = m_Ambient;
	
    float d = 1.f / 1000.f;
	ID3D11ShaderResourceView* pResource = nullptr;
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
		//case RenderMode::Depth:
		//{
		//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		//	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		//	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		//	shaderResourceViewDesc.Texture2D.MipLevels = 1;
		//
		//	ID3D11Texture2D* db = mp_D3D->GetDeferredRenderTarget()->DepthStencilBuffer();
		//
		//	mp_D3D->mp_Device->CreateShaderResourceView(db, &shaderResourceViewDesc, &pResource);
		//
		//	mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &pResource);
		//	cb.Ambient = Color(1, 1, 1, 1);
		//	break;
		//}
		case RenderMode::Depth:
		{
			mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1,
				&mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews()[m_RenderMode]
			);
			cb.Ambient = Color(d, d, d, 1);
			break;
		}


		case RenderMode::Light: {
			mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &mp_D3D->GetLightRT()->GetShaderResourceViews()[0]);
			cb.Ambient = Color(d, d, d, 1);
			break;
		}
        case RenderMode::BlurredLightH:
        {
			mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &mp_D3D->GetLightRT()->GetShaderResourceViews()[1]);
            cb.Ambient = Color(d, d, d, 1);
            break;
        }
		default:
		{
			mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1,
				&mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews()[m_RenderMode]
			);
			cb.Ambient = Color(1, 1, 1, 1);
			break;
		}
	}
	mp_ShaderProgramQuad->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramQuad->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	// do 3D rendering on the back buffer here
	_RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));

	if (pResource)
		pResource->Release();
}

void RenderManager::RenderDeferredBufferAmbientOnly()
{
	mp_D3D->BindSecondPassBuffer();
	mp_ShaderProgramQuad->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();

	Matrix4x4 M = Matrix4x4::Scale(2, 2, 1);
	QuadCB& cb = mp_ShaderProgramQuad->CB()->BufferData();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.Ambient = m_Ambient;
	mp_ShaderProgramQuad->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramQuad->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	mp_D3D->mp_DeviceContext->PSSetShaderResources(
		0,
		1,
		&mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews()[RenderMode::Diffuse]
	);

	// do 3D rendering on the back buffer here
	_RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
}

void RenderManager::RenderSecondPassBuffer()
{
	mp_ShaderProgramQuad->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();

	Matrix4x4 M = Matrix4x4::Scale(2, 2, 1);
	QuadCB& cb = mp_ShaderProgramQuad->CB()->BufferData();
	cb.ModelMatrix = Matrix4x4::Transpose(M);
	cb.Ambient = m_Ambient;
	mp_ShaderProgramQuad->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramQuad->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	mp_D3D->mp_DeviceContext->PSSetShaderResources(
		0,
		1,
		&mp_D3D->GetSecondPassRenderTarget()->GetShaderResourceViews()[0]
	);

	// do 3D rendering on the back buffer here
	_RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
}

void RenderManager::PrepShadowCastingLightPass()
{
	mp_D3D->GetLightRT()->ClearRenderTarget(mp_D3D->mp_DeviceContext);
	mp_D3D->EnableFrontFaceCulling();
	mp_ShaderProgramShadowCastingLight->BindShader();
	mp_D3D->EnableDepth();
	mp_D3D->DisableAlpha();
	// Bind the first render target of the light RT only
	mp_D3D->GetLightRT()->BindRT(mp_D3D->mp_DeviceContext, 0);
	mp_D3D->GetLightRT()->SetViewport(mp_D3D->mp_DeviceContext);
}

void RenderManager::PrepShadowCastingLightFinal()
{
    mp_D3D->BindSecondPassBuffer();
	mp_ShaderProgramShadowAddLight->BindShader();

	ID3D11ShaderResourceView** pTextures = mp_D3D->GetDeferredRenderTarget()->GetShaderResourceViews();
	UINT deferredNumViews = mp_D3D->GetDeferredRenderTarget()->GetNumViews();
	mp_D3D->mp_DeviceContext->PSSetShaderResources(0, deferredNumViews, pTextures);

	ID3D11ShaderResourceView* pShadowMap = mp_D3D->mp_LightRenderTarget->GetShaderResourceViews()[0]; // Grab the Light RT's first SRV (blurred shadow map)
	mp_D3D->mp_DeviceContext->PSSetShaderResources(deferredNumViews, 1, &pShadowMap);

	mp_D3D->DisableDepth();
	mp_D3D->EnableAlpha();
}

void RenderManager::ClearScreen(void)
{
	mp_D3D->ClearBackBuffer(m_ClearColor);
	mp_D3D->ClearDeferredBuffer(m_ClearColor);
	mp_D3D->ClearSecondPassBuffer(m_ClearColor);
	mp_D3D->ClearLightBuffer(m_ClearColor);
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
	cb.OverrideColor = Color(pMeshComp->GetOverrideColor());
	cb.SpecularValues = pMeshComp->GetSpecularValues();
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
	_RenderScene(pMeshComp->GetScene());
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
	cb.Ambient = m_Ambient;

	mp_ShaderProgramDeferredFinal->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);

	// set the new values for the constant buffer
	mp_ShaderProgramDeferredFinal->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	// do 3D rendering to the currently bound buffer here
	_RenderScene(pPointLightComp->GetScene());
}

void RenderManager::RenderObjectToLightShadowMap(const GameObject& goLight, const GameObject& go)
{
	if (!_GameObjectHasRenderableComponent(go))
		return;

	const TransformComponent * pTransComp = go.GetComponent<TransformComponent>();
	const MeshComponent * pMeshComp = go.GetComponent<MeshComponent>();

	const DirectionalLightComponent * pDirLightComp = goLight.GetComponent<DirectionalLightComponent>();

	ShadowCB& cb = mp_ShaderProgramShadowCastingLight->CB()->BufferData();
	cb.MatFinal = pDirLightComp->GetViewPerspMatrix() * pTransComp->GetTransform();
	cb.MatFinal.Transpose();

	mp_ShaderProgramShadowCastingLight->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
	// set the new values for the constant buffer
	mp_ShaderProgramShadowCastingLight->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	// do 3D rendering to the currently bound buffer here
	_RenderScene(pMeshComp->GetScene());
}

void RenderManager::AddSCLInfluenceToScene(const GameObject & pGOCamera, GameObject & goLight)
{
	const TransformComponent * pTransComp = goLight.GetComponent<TransformComponent>();
	DirectionalLightComponent * pLightComp = goLight.GetComponent<DirectionalLightComponent>();
	ShadowAddLightCB& cb = mp_ShaderProgramShadowAddLight->CB()->BufferData();

	cb.ShadowMatrix = pLightComp->GetShadowMatrix();
	cb.ModelMatrix = Matrix4x4::Scale(2, 2, 1);
	cb.CameraPosition = pGOCamera.GetComponent<TransformComponent>()->WorldPosition();
	cb.LightPosition = pTransComp->WorldPosition();
	cb.LightPosition.w = pLightComp->LightA();
	cb.LightColor = pLightComp->GetColor();
	cb.LightColor.a = pLightComp->LightB();
	cb.LIDHW.x = pLightComp->Intensity();
	cb.LIDHW.y = pLightComp->Distance();
	cb.LIDHW.z = float(m_WindowSettings.Height);
	cb.LIDHW.w = float(m_WindowSettings.Width);

	mp_ShaderProgramShadowAddLight->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
	// set the new values for the constant buffer
	mp_ShaderProgramShadowAddLight->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

	// do 3D rendering to the currently bound buffer here
	_RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
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
			break;
		case 3:
			mp_ShaderProgramShadowCastingLight->Initialize(mp_D3D->mp_Device, filePath);
			break;
		case 4:
			mp_ShaderProgramShadowAddLight->Initialize(mp_D3D->mp_Device, filePath);
			break;
		case 5:
			mp_ShaderProgramGaussianBlur->Initialize(mp_D3D->mp_Device, filePath);
			break;
        case 6:
        {
            std::vector<ShaderProgram<Blur2CB>::ShaderMetaData> smd = { { filePath, "HorizontalBlurCS", Shader::ShaderType::ShaderType_Compute } };
            mp_ShaderProgramShadowMapHBlur->Initialize(mp_D3D->mp_Device, smd);
            break;
        }
        case 7:
        {
            std::vector<ShaderProgram<Blur2CB>::ShaderMetaData> smd = { { filePath, "VerticalBlurCS", Shader::ShaderType::ShaderType_Compute } };
            mp_ShaderProgramShadowMapVBlur->Initialize(mp_D3D->mp_Device, smd);
            break;
        }
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

void RenderManager::PrevRenderMode()
{
    m_RenderMode = RenderMode(int(m_RenderMode) - 1);
    if (m_RenderMode < 0)
        m_RenderMode = RenderMode(RenderMode::NUM_MODES - 1);
}

void RenderManager::PrepParticlePass()
{
}

void RenderManager::RenderParticles(const GameObject& pGOCamera, const GameObject& pGO)
{

}

void RenderManager::BlurDepthMap2(GameObject & goLight)
{
	enum HoV : UINT {
		HoV_Horizontal = 0,
		HoV_Vertical
	};

	mp_D3D->EnableBackFaceCulling();
	mp_ShaderProgramGaussianBlur->BindShader();
	mp_D3D->DisableDepth();
	mp_D3D->DisableAlpha();

	const DirectionalLightComponent * pLightComp = goLight.GetComponent<DirectionalLightComponent>();
	RenderTarget* pRenderTarget = mp_D3D->GetLightRT();

	BlurCB& cb = mp_ShaderProgramGaussianBlur->CB()->BufferData();
    std::vector<FLOAT> weights = pLightComp->Weights();
    std::vector<FLOAT> offsetsX = pLightComp->OffsetsX();
    std::vector<FLOAT> offsetsY = pLightComp->OffsetsY();
    UINT curWeightVec = 0;
    for (UINT i = 0; i < weights.size(); i += 4) {
        cb.Weights[curWeightVec] = Vector3D(
            weights[i + 0],
            weights[i + 1],
            weights[i + 2],
            weights[i + 3]
        );
        cb.Offsets.X[curWeightVec] = Vector3D(
            offsetsX[i + 0],
            offsetsX[i + 1],
            offsetsX[i + 2],
            offsetsX[i + 3]
        );
        cb.Offsets.Y[curWeightVec] = Vector3D(
            offsetsY[i + 0],
            offsetsY[i + 1],
            offsetsY[i + 2],
            offsetsY[i + 3]
        );
        ++curWeightVec;
    }

    cb.BlurAmount = pLightComp->BlurAmount();


	// Horizontal Blur
	{
		pRenderTarget->SetViewport(mp_D3D->mp_DeviceContext);
		pRenderTarget->BindRT(mp_D3D->mp_DeviceContext, 1);  // Bind the Light RT's second RTV (the horizontally shadow map) for drawing
		pRenderTarget->ClearDepthStencilView(mp_D3D->mp_DeviceContext);

		cb.HorizontalOrVertical = HoV::HoV_Horizontal;
        
		mp_ShaderProgramGaussianBlur->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
		mp_ShaderProgramGaussianBlur->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

		// Bind the Light RT's first SRV (the shadow map) for horizontal blurring to the second RTV
		mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &pRenderTarget->GetShaderResourceViews()[0]);

		// do 3D rendering to the currently bound buffer here
		_RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
	}

	// Vertical Blur
	{
		pRenderTarget->ClearDepthStencilView(mp_D3D->mp_DeviceContext);
		pRenderTarget->BindRT(mp_D3D->mp_DeviceContext, 0); // Bind the Light RT's first RTV (the shadow map) for drawing

		cb.HorizontalOrVertical = HoV::HoV_Vertical;

		mp_ShaderProgramGaussianBlur->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
		mp_ShaderProgramGaussianBlur->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

		// Bind the Light RT's second SRV for veritcal blurring to the first RTV
        mp_D3D->mp_DeviceContext->PSSetShaderResources(0, 1, &pRenderTarget->GetShaderResourceViews()[1]);

		// do 3D rendering to the currently bound buffer here
		_RenderScene(INFECT_RESOURCES.GetScene(QUAD_PRIMITIVE));
	}
}

void RenderManager::BlurDepthMap(GameObject & goLight)
{
    mp_D3D->EnableBackFaceCulling();
    mp_D3D->DisableDepth();
    mp_D3D->DisableAlpha();

    //D3D10_TECHNIQUE_DESC;
    const DirectionalLightComponent * pLightComp = goLight.GetComponent<DirectionalLightComponent>();
    RenderTarget* pRenderTarget = mp_D3D->GetLightRT();
    std::vector<FLOAT> weights = pLightComp->Weights();



    UINT negOne = -1,
        N = 1024 / 256;

    // Horizontal Blur
    {
        mp_ShaderProgramShadowMapHBlur->BindShader();
        Blur2CB& cb = mp_ShaderProgramShadowMapHBlur->CB()->BufferData();
        
        UINT curWeightVec = 0;
        for (UINT i = 0; i < weights.size(); i += 4) {
            cb.Weights[curWeightVec] = Vector3D(
                weights[i + 0],
                weights[i + 1],
                weights[i + 2],
                weights[i + 3]
            );
            ++curWeightVec;
        }
        //for (UINT i = 0; i < weights.size(); ++i) {
        //    cb.Weights[i] = weights[i];
        //}

        pRenderTarget->SetViewport(mp_D3D->mp_DeviceContext);
        pRenderTarget->BindRT(mp_D3D->mp_DeviceContext, 1);  // Bind the Light RT's second RTV (the horizontally shadow map) for drawing
        pRenderTarget->ClearDepthStencilView(mp_D3D->mp_DeviceContext);

        mp_ShaderProgramShadowMapHBlur->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
        mp_ShaderProgramShadowMapHBlur->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

        mp_D3D->mp_DeviceContext->CSSetUnorderedAccessViews(0, 1, &pRenderTarget->GetUAVs()[1], &negOne);
        mp_D3D->mp_DeviceContext->CSSetShaderResources(0, 1, &pRenderTarget->GetShaderResourceViews()[0]);

        mp_D3D->mp_DeviceContext->Dispatch(N, 1024, 1);
    }

    // Clear the input texture from the compute shader
    ID3D11ShaderResourceView* nullSRV[1] = { 0 };
    mp_D3D->mp_DeviceContext->CSSetShaderResources(0, 1, nullSRV);

    // Unbind output from compute shader
    ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
    mp_D3D->mp_DeviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

    // Disable compute shader.
    mp_D3D->mp_DeviceContext->CSSetShader(0, 0, 0);

    // Vertical Blur
    {
        mp_ShaderProgramShadowMapVBlur->BindShader();
        Blur2CB& cb = mp_ShaderProgramShadowMapVBlur->CB()->BufferData();
        UINT curWeightVec = 0;
        for (UINT i = 0; i < weights.size(); i += 4) {
            cb.Weights[curWeightVec] = Vector3D(
                weights[i + 0],
                weights[i + 1],
                weights[i + 2],
                weights[i + 3]
            );
            ++curWeightVec;
        }
        //for (UINT i = 0; i < weights.size(); ++i) {
        //    cb.Weights[i] = weights[i];
        //}

        pRenderTarget->ClearDepthStencilView(mp_D3D->mp_DeviceContext);
        pRenderTarget->BindRT(mp_D3D->mp_DeviceContext, 0); // Bind the Light RT's first RTV (the shadow map) for drawing

        mp_ShaderProgramShadowMapVBlur->CB()->SetConstantBuffer(mp_D3D->mp_DeviceContext);
        mp_ShaderProgramShadowMapVBlur->CB()->UpdateSubresource(mp_D3D->mp_DeviceContext);

        mp_D3D->mp_DeviceContext->CSSetUnorderedAccessViews(0, 1, &pRenderTarget->GetUAVs()[0], &negOne);
        mp_D3D->mp_DeviceContext->CSSetShaderResources(0, 1, &pRenderTarget->GetShaderResourceViews()[1]);

        mp_D3D->mp_DeviceContext->Dispatch(1024, N, 1);
    }

    // Clear the input texture from the compute shader
    mp_D3D->mp_DeviceContext->CSSetShaderResources(0, 1, nullSRV);

    // Unbind output from compute shader
    mp_D3D->mp_DeviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);

    // Disable compute shader.
    mp_D3D->mp_DeviceContext->CSSetShader(0, 0, 0);
}
