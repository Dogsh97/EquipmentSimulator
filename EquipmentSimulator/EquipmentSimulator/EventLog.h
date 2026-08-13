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

class EventLog {
	private:
		CommandType commandType;
		bool success;
		CommandResultType type;
	public:
		EventLog(CommandType commandtype, bool success, CommandResultType type);
		CommandType GetCommandType();
		bool IsSuccess();
		void Print();

};