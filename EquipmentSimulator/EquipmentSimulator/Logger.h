#pragma once
#include <iostream>
#include <vector>
#include "EventLog.h"

class Logger {
	private:
		std::vector<std::string> logs;
		std::vector<EventLog> eventLogs;

	public:
		Logger();
		
		void Log(const std::string& message);
		void PrintLog() const;
		void AddEventLog(EventLog eventlog);
		void PrintEventLogs();
};