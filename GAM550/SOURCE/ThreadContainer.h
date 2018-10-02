/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef THREAD_CONTAINER_H
#define THREAD_CONTAINER_H

class JobManager;

// A ThreadContainer object holds a reference to it's std::thread and is responsible for 
// spawning the thread. The RunThread method is the loop body of the thread and should be
// filled out by any derived classes.
// The Type member and GetType() method should be overridden by derived classes as well
// NOTE: The thread should NOT join() itself.
class ThreadContainer
{
protected:
	std::thread * mp_Thread;				// Pointer to the thread spawned by this container
	bool m_Terminate;						// When true, this thread will exit it's RunThread loop
	bool m_Busy;							// 
	const ThreadCommand* mp_CurrentCommand;	// Pointer to the current command being processed

public:
	static const ThreadType Type = ThreadType::AnyThread;
	virtual ThreadType GetType() const { return Type; }

	ThreadContainer() :
		mp_Thread(nullptr),
		m_Terminate(false), m_Busy(false),
		mp_CurrentCommand(nullptr)
	{};
	virtual ~ThreadContainer() {};

	// Derived classes are responsible for creating a loop that will process commands
	// or ask for new ones until the thread is terminated
	virtual void RunThread() = 0;
	// Derived classes are responsible for creating a new std::thread object
	virtual std::thread& Spawn() = 0;

	// Joins the thread and closes it. Do NOT invoke within RunThread or any method that a ThreadCommand can invoke.
	inline void Join() { mp_Thread->join(); }
	// Causes the thread to end at the beginning of it's next RunThread loop
	inline void Terminate() { m_Terminate = true; }
	// Reports back whether or not the thread is busy doing work
	inline bool IsBusy() { return m_Busy; }
};

//template<typename ThreadWorker, ThreadType type>
//class TypedThreadContainer : public ThreadContainer {
//protected:
//	ThreadWorker & m_Worker;	// Reference to the ThreadWorker object belonging to this thread
//
//public:
//	static const ThreadType Type = type;
//	virtual ThreadType GetType() const { return Type; }
//
//	TypedThreadContainer(JobManager& jobManager, ThreadWorker& worker) :
//		ThreadContainer(jobManager),
//		m_Worker(worker) {}
//	virtual ~TypedThreadContainer() {};
//
//	// Derived classes are responsible for creating a loop that will process commands
//	// or ask for new ones until the thread is terminated
//	virtual void RunThread() = 0; 
//	/* Derived classes are responsible for creating a new std::thread object
//	/ Typical implementation:
//	/	mp_Thread = new std::thread(&TypedThreadContainer::RunThread, this);
//	/	return mp_Thread;
//	*/
//	virtual std::thread& Spawn() = 0;
//
//	// Returns a reference to the ThreadWorker template object
//	inline ThreadWorker& Worker() { return m_Worker; }
//};

template<ThreadType type>
class TypedThreadContainer : public ThreadContainer {
public:
	static const ThreadType Type = type;
	virtual ThreadType GetType() const { return Type; }

	TypedThreadContainer() :
		ThreadContainer()
	{}
	virtual ~TypedThreadContainer() {};

	// Derived classes are responsible for creating a loop that will process commands
	// or ask for new ones until the thread is terminated
	virtual void RunThread() = 0;
	/* Derived classes are responsible for creating a new std::thread object
	/ Typical implementation:
	/	mp_Thread = new std::thread(&TypedThreadContainer::RunThread, this);
	/	return *mp_Thread;
	*/
	virtual std::thread& Spawn() = 0;
};


#endif