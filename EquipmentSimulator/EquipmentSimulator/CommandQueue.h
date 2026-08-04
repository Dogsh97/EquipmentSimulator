#pragma once
#include <queue>
#include "Command.h"

class CommandQueue {
	private:
		std::queue<Command> commandQueue;
		
	public:
		CommandQueue();
		Command GetCommand();
		void PushCommand(Command command);
		void PrintCommandInfo() const;
		void PopCommand();
		bool CommandDetected() const;
};