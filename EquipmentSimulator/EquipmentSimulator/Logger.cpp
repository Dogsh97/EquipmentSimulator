#include <iostream>
#include "Logger.h"

Logger::Logger()
{
}

void Logger::Log(const std::string& message) {
	logs.push_back(message);
}

void Logger::PrintLog() const {
	for (const auto& log : logs) {
		std::cout << log << '\n';
	}
}

void Logger::AddEventLog(EventLog eventlog) {
	eventLogs.push_back(eventlog);
}

void Logger::PrintEventLogs() {
	for (int i = 0; i < eventLogs.size(); ++i) {
		eventLogs[i].Print();
	}
}