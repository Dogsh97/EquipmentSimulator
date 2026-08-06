#include <iostream>
#include "EventLog.h"
#include <string>

EventLog::EventLog(CommandType commandtype, bool success)
	: commandType(commandtype),
	success(success)
{
}

CommandType EventLog::GetCommandType() {
	return commandType;
}

bool EventLog::IsSuccess() {
	return success;
}

void EventLog::Print() {
	switch (commandType) {
	case CommandType::None:
		std::cout << "None";
		break;

	case CommandType::Initialize:
		std::cout << "Initialize";
		break;

	case CommandType::SetRecipe:
		std::cout << "SetRecipe";
		break;

	case CommandType::CompleteInitialization:
		std::cout << "CompleteInitialization";
		break;

	case CommandType::LoadWafer:
		std::cout << "LoadWafer";
		break;

	case CommandType::Start:
		std::cout << "Start";
		break;

	case CommandType::Complete:
		std::cout << "Complete";
		break;

	case CommandType::RaiseError:
		std::cout << "RaiseError";
		break;

	case CommandType::Reset:
		std::cout << "Reset";
		break;

	case CommandType::PrintState:
		std::cout << "PrintState";
		break;
	}

	if (success) {
		std::cout << ": Success\n";
	}
	else {
		std::cout << ": Fail\n";
	}
}