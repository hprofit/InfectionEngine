#include "Stdafx.h"

JobManager::JobManager()
{}

JobManager::~JobManager()
{
	for (int i = 0; i < ThreadType::NUMTHREADTYPES; ++i) {
		delete m_CommandQueues[i];
	}
}

void JobManager::Init()
{
	for (int i = 0; i < ThreadType::NUMTHREADTYPES-1; ++i) {
		m_ThreadContainers[i] = nullptr;
	}
	for (int i = 0; i < ThreadType::NUMTHREADTYPES; ++i) {
		m_CommandQueues[i] = new ThreadCommandList();
	}
}

void JobManager::RegisterThreadContainer(ThreadContainer* tjc)
{
	m_ThreadContainers[tjc->GetType()] = tjc;
	tjc->Spawn();
}

ThreadContainer * JobManager::GetGenericThreadContainer(ThreadType type)
{
	return m_ThreadContainers[type];
}

void JobManager::AddNewJob(ThreadCommand * cmd)
{
	m_CommandQueues[cmd->type]->AddCommand(cmd);
}

void JobManager::FinishedJob(ThreadType type)
{
	m_CommandQueues[type]->PopTopCommand();
}

bool JobManager::HasJobs(ThreadType type)
{
	return m_CommandQueues[type]->HasCommands();
}

bool JobManager::HasAnyJobs()
{
	bool hasAnyJobs = false;
	for (int i = 0; i < ThreadType::NUMTHREADTYPES; ++i) {
		hasAnyJobs = HasJobs(ThreadType(i)) ? true : hasAnyJobs;
	}
	return hasAnyJobs;
}

const ThreadCommand * JobManager::GetThreadJob(ThreadType type)
{
	return m_CommandQueues[type]->GetTopCommand();
}
