#pragma once
#include <iostream>
#include <vector>

class Logger {
	private:
		std::vector<std::string> logs;

	public:
		Logger();

		void Log(const std::string& message);

		void PrintLog() const;
};