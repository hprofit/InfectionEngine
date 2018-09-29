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
	//INFECT_RENDERER.BindBackBuffer();
	//INFECT_GOM.RenderCameras();						// Render all game objects


	INFECT_RENDERER.BindDeferredBuffer();
	INFECT_GOM.RenderCameras();
	INFECT_RENDERER.BindBackBuffer();
	//INFECT_RENDERER.RenderDeferredBuffer(); // Don't use
	//INFECT_RENDERER.RenderDeferredBufferAmbientOnly();
	INFECT_RENDERER.PrepDeferredFinal();
	INFECT_GOM.RenderLights();


	INFECT_RENDERER.PresentFrameToScreen();			// Swap window buffer
	INFECT_THREAD_JOBS.AddNewJob(new StartSimulationCommand(*INFECT_THREAD_JOBS.GetThreadContainer<SimulationThreadContainer>(ThreadType::SimThread)));
	return true;
}

bool RenderTerminateTerminate::execute() const
{
	if (tCont.IsBusy())
		return false;
	tCont.Terminate();
	return true;
}
