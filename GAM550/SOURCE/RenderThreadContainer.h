/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef RENDER_THREAD_CONTAINER_H
#define RENDER_THREAD_CONTAINER_H

class RenderThreadContainer : 
	public TypedThreadContainer<ThreadType::RenderThread>{
public:
	virtual ThreadType GetType() const { return Type; }

	RenderThreadContainer() :
		TypedThreadContainer()
	{}
	virtual ~RenderThreadContainer() {};

	virtual void RunThread();
	virtual std::thread& Spawn();
};

class RenderThreadCommand : public ThreadCommand {
public:
	RenderThreadContainer & tCont;
	RenderThreadCommand(RenderThreadContainer& _tCont) :
		ThreadCommand(ThreadType::RenderThread), tCont(_tCont)
	{}
	virtual bool execute() const = 0;
};

class StartRenderCommand : public RenderThreadCommand {
public:

	StartRenderCommand(RenderThreadContainer& _tCont) :
		RenderThreadCommand(_tCont)
	{}
	virtual bool execute() const;
};

class RenderTerminateTerminate : public RenderThreadCommand {
public:
	RenderTerminateTerminate(RenderThreadContainer& _tCont) :
		RenderThreadCommand(_tCont)
	{}
	virtual bool execute() const;
};

#endif