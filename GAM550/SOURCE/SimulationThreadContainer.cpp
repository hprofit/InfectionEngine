#include "Stdafx.h"

void SimulationThreadContainer::RunThread()
{
	while (!m_Terminate) {
		if (!mp_CurrentCommand) {
			if (INFECT_THREAD_JOBS.HasJobs(Type)) {
				mp_CurrentCommand = INFECT_THREAD_JOBS.GetThreadJob(Type);
				std::cout << "Simulation Thread : Work received." << std::endl;
			}
			std::cout << "Simulation Thread : Idling..." << std::endl;
		}

		if (mp_CurrentCommand) {
			if (mp_CurrentCommand->execute()) {
				std::cout << "Simulation Thread : Work completed." << std::endl;
				INFECT_THREAD_JOBS.FinishedJob(Type);
				mp_CurrentCommand = nullptr;
			}
			std::cout << "Simulation Thread : Working..." << std::endl;
		}
	}
	std::cout << "Simulation Thread : Terminating." << std::endl;
}

std::thread & SimulationThreadContainer::Spawn()
{
	mp_Thread = new std::thread(&SimulationThreadContainer::RunThread, this);
	return *mp_Thread;
}

bool StartSimulationCommand::execute() const
{
	return false;
}

bool SimulationTerminateTerminate::execute() const
{
	if (tCont.IsBusy())
		return false;
	tCont.Terminate();
	return true;
}
