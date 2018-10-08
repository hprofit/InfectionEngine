#include "Stdafx.h"

ThreadCommandList::CommandNode::~CommandNode()
{
	delete mp_Cmd;
}

void ThreadCommandList::CommandNode::Clean()
{
	if (mp_Next)
		mp_Next->Clean();
	delete mp_Cmd;
	delete mp_Next;
}



ThreadCommandList::~ThreadCommandList()
{
	if (mp_Head)
		mp_Head->Clean();
	mp_Head = mp_Tail = 0;
}

void ThreadCommandList::AddCommand(ThreadCommand * cmd)
{
	CommandNode* newCommandNode = new CommandNode(cmd);
	// If the head is null, the tail is too, meaning we have an empty list
	if (!mp_Head) {
		mp_Head = mp_Tail = newCommandNode;
	}
	else {
		mp_Tail->mp_Next = newCommandNode;
		mp_Tail = mp_Tail->mp_Next;
	}
}

ThreadCommand * ThreadCommandList::GetTopCommand()
{
	return mp_Head->mp_Cmd;
}

void ThreadCommandList::PopTopCommand()
{
	CommandNode * newHead = mp_Head->mp_Next;
	delete mp_Head;
	mp_Head = newHead;
}

bool ThreadCommandList::HasCommands()
{
	return mp_Head;
}
