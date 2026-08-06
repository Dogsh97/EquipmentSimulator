#pragma once
#include "Command.h"

class EventLog {
	private:
		CommandType commandType;
		bool success;

	public:
		EventLog(CommandType commandtype, bool success);
		CommandType GetCommandType();
		bool IsSuccess();
		void Print();

};