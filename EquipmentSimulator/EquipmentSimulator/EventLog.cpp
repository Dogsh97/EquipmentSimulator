#include <iostream>
#include "EventLog.h"
#include <string>

EventLog::EventLog(CommandType commandtype, bool success, CommandResultType type)
	: commandType(commandtype),
	success(success),
	type(type)
{
}

CommandType EventLog::GetCommandType() const{
	return commandType;
}

bool EventLog::IsSuccess() const{
	return success;
}

std::string CommandResultTypeToString(CommandResultType type) {
	switch (type)
	{
	case CommandResultType::ParameterValidationFailed:
		return "ParameterValidationFailed";
	case CommandResultType::CanExecuteFailed:
		return "CanExecuteFailed";
	case CommandResultType::InterlockFailed:
		return "InterlockFailed";
	case CommandResultType::CommandExecutionFailed:
		return "CommandExecutionFailed";
	case CommandResultType::PostValidationFailed:
		return "PostValidationFailed";
	}

	return "Unknown";
	
}

void EventLog::Print() const{
	std::cout << "CommandType : " << CommandTypeToString(commandType) << "\n";

	if (success) {
		std::cout << "Result : Success\n";
		return;
	}

	std::cout << "Result : Fail\n";
	std::cout << "Reason : " <<	 CommandResultTypeToString(type) << "\n";
	
}