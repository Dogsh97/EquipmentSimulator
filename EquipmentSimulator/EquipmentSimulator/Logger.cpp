#include <iostream>
#include "Logger.h"

Logger::Logger()
{
}

void Logger::Log(const std::string& message) {
	logs.push_back(message);
}

void Logger::PrintLog() const {
	std::cout << "=====  Logs =====\n";
	for (const auto& log : logs) {
		std::cout << log << '\n';
	}
}

void Logger::AddEventLog(EventLog eventlog) {
	eventLogs.push_back(eventlog);
}

void Logger::PrintEventLogs() {
	std::cout << "===== Event Logs =====\n";
	for (int i = 0; i < eventLogs.size(); ++i) {
		std::cout << "[" << i+1 << "]\n";
		eventLogs[i].Print();
	}
}

void Logger::ResetEventLogs() {
	eventLogs.clear();
}