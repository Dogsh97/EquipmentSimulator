#pragma once
#include "Command.h"

enum class CommandResultType{
	Success,
	ParameterValidationFailed,
	CanExecuteFailed,
	InterlockFailed,
	CommandExecutionFailed,
	PostValidationFailed
};

std::string CommandResultTypeToString(CommandResultType type);

class EventLog {
	private:
		CommandType commandType;
		bool success;
		CommandResultType type;
	public:
		EventLog(CommandType commandtype, bool success, CommandResultType type);
		CommandType GetCommandType() const;
		bool IsSuccess() const;
		void Print() const;

};