/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef THREAD_COMMAND_LIST_H
#define THREAD_COMMAND_LIST_H

class ThreadCommand;

class ThreadCommandList {
protected:
	class CommandNode {
	public:
		ThreadCommand * mp_Cmd;
		CommandNode* mp_Next;

		CommandNode(ThreadCommand* cmd) :
			mp_Cmd(cmd), mp_Next(nullptr) {}
		~CommandNode();

		void Clean();
	};

	CommandNode * mp_Head = nullptr;
	CommandNode* mp_Tail = nullptr;

public:
	ThreadCommandList() :
		mp_Head(nullptr), mp_Tail(nullptr) {}
	~ThreadCommandList();

	// Adds a new command to the list
	void AddCommand(ThreadCommand* cmd);
	// Returns the ThreadCommand from the top of the list
	ThreadCommand* GetTopCommand();
	// Removes the top command from the list
	void PopTopCommand();
	// Returns true if there are any commands in the list
	bool HasCommands();
};

#endif