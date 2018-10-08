#pragma once
#ifndef COMMAND_H
#define COMMAND_H

#include "ThreadType.h"

class Command {
public:
	bool priority = false;
	InfectGUID mGUID;

	Command();

	virtual bool execute() const = 0;
};

class ThreadCommand : public Command 
{
public:
	ThreadType type;

	ThreadCommand(ThreadType _type) : Command(), type(_type) {};
	virtual bool execute() const = 0;
};

struct hasPriority {
	bool operator()(const Command* a, const Command* b) {
		// If neither has priority, return comparison of ids, if a's id is lower than b's id
		// a was created earlier and should go before b 
		if (!a->priority && !b->priority)
			return a->mGUID > b->mGUID;
		// If both have priority, return comparison of ids, if a's id is lower than b's id
		// a was created earlier and should go before b 
		else if (a->priority && b->priority)
			return a->mGUID < b->mGUID;
		// If a has priority but b does not, return true indicating that a should be placed before b
		// else return false meaning b comes before a
		return (a->priority && !b->priority);
	}
};


#endif