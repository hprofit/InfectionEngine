#include "Stdafx.h"

void RenderThreadContainer::RunThread()
{
	while (!m_Terminate) {
		if (!mp_CurrentCommand) {
			if (INFECT_THREAD_JOBS.HasJobs(Type)) {
				mp_CurrentCommand = INFECT_THREAD_JOBS.GetThreadJob(Type);
				//std::cout << "Render Thread : Work received." << std::endl;
			}
			//std::cout << "Render Thread : Idling..." << std::endl;
		}

		if (mp_CurrentCommand) {
			if (mp_CurrentCommand->execute()) {
				//std::cout << "Render Thread : Work completed." << std::endl;
				INFECT_THREAD_JOBS.FinishedJob(Type);
				mp_CurrentCommand = nullptr;
			}
			//std::cout << "Render Thread : Working..." << std::endl;
		}
	}
	std::cout << "Render Thread : Terminating." << std::endl;
}

std::thread & RenderThreadContainer::Spawn()
{
	mp_Thread = new std::thread(&RenderThreadContainer::RunThread, this);
	return *mp_Thread;
}

bool StartRenderCommand::execute() const
{
	INFECT_RENDERER.ClearScreen();					// Clear the window buffer
	
	// Forward Rendering, DO NOT USE
	//INFECT_RENDERER.BindBackBuffer();
	//INFECT_GOM.RenderCameras();						// Render all game objects
	// END DO NOT USE

	INFECT_GOM.FillShadowCastingLightsShadowMaps();		// Requires visibility check

	//INFECT_RENDERER.PrepDeferredPass();
	//INFECT_GOM.RenderCameras();					// Requires visibility check

	// Render the G BUffers individually OR first shadow casting light's depth map - DEBUG ONLY
	if (INFECT_RENDERER.CurrentRenderMode() != RenderMode::Final) {
		//INFECT_RENDERER.BindBackBuffer();
		//INFECT_RENDERER.RenderDebugBuffers();
	}
	// Render the whole deferred shading and lighting
	else {
		// Render ambient lighting to second buffer
		INFECT_RENDERER.BindSecondPassBuffer();
		INFECT_RENDERER.RenderDeferredBufferAmbientOnly();

		INFECT_RENDERER.PrepShadowCastingLightFinal();
		INFECT_GOM.AddLightFromShadowCastingLights();

		// Render each non-shadow casting light to the second buffer using the deferred second pass
		INFECT_RENDERER.PrepDeferredFinal();
		INFECT_GOM.RenderLights();

		// Render the contents of the second buffer to the back buffer
		INFECT_RENDERER.BindBackBuffer();
		INFECT_RENDERER.RenderSecondPassBuffer();
	}


	INFECT_RENDERER.PresentFrameToScreen();			// Swap window buffer
	INFECT_THREAD_JOBS.AddNewJob(new StartSimulationCommand(*INFECT_THREAD_JOBS.GetThreadContainer<SimulationThreadContainer>(ThreadType::SimThread)));
	return true;
}

bool RenderTerminateTerminate::execute() const
{
	//if (tCont.IsBusy())
	//	return false;
	tCont.Terminate();
	return true;
}
