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