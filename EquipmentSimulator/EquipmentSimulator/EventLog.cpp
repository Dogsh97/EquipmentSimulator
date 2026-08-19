#include <iostream>
#include "EventLog.h"
#include <string>

EventLog::EventLog(CommandType commandtype, bool success, CommandResultType type)
	: commandType(commandtype),
	success(success),
	type(type)
{
}

CommandType EventLog::GetCommandType() {
	return commandType;
}

bool EventLog::IsSuccess() {
	return success;
}

void EventLog::Print() {
	std::cout << "Command : ";
	switch (commandType) {

	case CommandType::None:
		std::cout << "None\n";
		break;

	case CommandType::Initialize:
		std::cout << "Initialize\n";
		break;

	case CommandType::SetRecipe:
		std::cout << "SetRecipe\n";
		break;

	case CommandType::CompleteInitialization:
		std::cout << "CompleteInitialization\n";
		break;

	case CommandType::LoadWafer:
		std::cout << "LoadWafer\n";
		break;

	case CommandType::Start:
		std::cout << "Start\n";
		break;

	case CommandType::Complete:
		std::cout << "Complete\n";
		break;

	case CommandType::RaiseError:
		std::cout << "RaiseError\n";
		break;

	case CommandType::Reset:
		std::cout << "Reset\n";
		break;

	case CommandType::PrintState:
		std::cout << "PrintState\n";
		break;
	}

	if (success) {
		std::cout << "Result : Success\n";
	}
	else {
		std::cout << "Result : Fail\n";
		std::cout << "Reason : ";
	}

	

	switch (type) {
		case CommandResultType::ParameterValidationFailed:
			std::cout << "ParameterValidationFailed\n";
			break;

		case CommandResultType::CanExecuteFailed:
			std::cout << "CanExecuteFailed\n";
			break;

		case CommandResultType::InterlockFailed:
			std::cout << "InterlockFailed\n";
			break;

		case CommandResultType::CommandExecutionFailed:
			std::cout << "CommandExecutionFailed\n";
			break;

		case CommandResultType::PostValidationFailed:
			std::cout << "PostValidationFailed\n";
			break;
	}	
}