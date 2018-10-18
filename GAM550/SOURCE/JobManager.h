/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef JOB_MANAGER_H
#define JOB_MANAGER_H

class ThreadCommandList;

class JobManager
{
protected:
	// Store a number of ThreadContainers equal the total number of thread types - 1 as the final thread type is 'Any'
	// and does not correspond to any specific ThreadContainer
	ThreadContainer * m_ThreadContainers[ThreadType::NUMTHREADTYPES-1];

	// Store a number of CommandNodeContainers equal to the total number of thread types
	// Note that the 'Any' ThreadCommandList does not correspond to any specific ThreadContainer and when processed, will
	// spawn a new mini thread
	ThreadCommandList* m_CommandQueues[ThreadType::NUMTHREADTYPES];

public:
	JobManager();
	~JobManager();

	// Initializes both arrays.
	void Init();

	bool HasRunningThreads() const;

	// Registers a spot in the ThreadContainer array according to what type the ThreadContainer is
	void RegisterThreadContainer(ThreadContainer* tjc);

	// Returns a type casted TypedThreadContainer pointer of the specified template type
	template <typename T>
	T* GetThreadContainer(ThreadType type) {
		return static_cast<T*>(m_ThreadContainers[type]);
	}

	// Returns a ThreadContainer pointer from the ThreadContainers array
	ThreadContainer* GetGenericThreadContainer(ThreadType type);
	// Adds the given ThreadCommand to the appropriate CommandQueue according to the type
	void AddNewJob(ThreadCommand * cmd);
	// Removes a command from the appropriate CommandQueue according to the type
	void FinishedJob(ThreadType type);
	// Returns true if the CommandQueue corresponding to the ThreadType has any jobs remaining
	bool HasJobs(ThreadType type);
	// Returns true if any CommandQueue any jobs remaining
	bool HasAnyJobs();
	// Returns the highest priority job from the CommandQueue corresponding to the ThreadType
	const ThreadCommand * GetThreadJob(ThreadType type);
};

#endif JOB_MANAGER_H