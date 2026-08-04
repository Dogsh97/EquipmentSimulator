#include <iostream>
#include "CommandQueue.h"
#include "Command.h"

CommandQueue::CommandQueue() {

}

Command CommandQueue::GetCommand() {
	if (CommandDetected()) {
		return commandQueue.front();
	}
	else {
		return Command(CommandType::None);
	}
}

void CommandQueue::PushCommand(Command command) {
	commandQueue.push(command);
}

void CommandQueue::PrintCommandInfo() const {
	std::queue<Command> temp;
	temp = commandQueue;

	while (!temp.empty()) {
		std::cout << "Command:";
		temp.front().PrintCommand();
		std::cout << "\n";
		temp.pop();
	}

}

void CommandQueue::PopCommand() {
	if (CommandDetected()) {
		commandQueue.pop();;
	}
	else {
		std::cout << "Queue is Empty";
	}
	
}

bool CommandQueue::CommandDetected() const {
	return !commandQueue.empty();
}