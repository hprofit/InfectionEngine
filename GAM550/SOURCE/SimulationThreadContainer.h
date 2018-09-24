/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef SIMULATION_THREAD_CONTAINER_H
#define SIMULATION_THREAD_CONTAINER_H

class SimulationThreadContainer : 
	public TypedThreadContainer<ThreadType::SimThread>{
public:
	virtual ThreadType GetType() const { return Type; }

	SimulationThreadContainer() :
		TypedThreadContainer()
	{}
	virtual ~SimulationThreadContainer() {};

	virtual void RunThread();
	virtual std::thread& Spawn();
};

class SimulationThreadCommand : public ThreadCommand {
public:
	SimulationThreadContainer & tCont;
	SimulationThreadCommand(SimulationThreadContainer& _tCont) :
		ThreadCommand(ThreadType::SimThread), tCont(_tCont)
	{}
	virtual bool execute() const = 0;
};

class StartSimulationCommand : public SimulationThreadCommand {
public:

	StartSimulationCommand(SimulationThreadContainer& _tCont) :
		SimulationThreadCommand(_tCont)
	{}
	virtual bool execute() const;
};

class SimulationTerminateTerminate : public SimulationThreadCommand {
public:
	SimulationTerminateTerminate(SimulationThreadContainer& _tCont) :
		SimulationThreadCommand(_tCont)
	{}
	virtual bool execute() const;
};

#endif