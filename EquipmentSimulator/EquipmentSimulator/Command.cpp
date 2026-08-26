#include <iostream>
#include "Command.h"

Command::Command(CommandType type)
	:type(type),
	recipeId(-1),
	processTime(-1),
	temperature(-1),
	waferId(-1),
	retryCount(0)
{
}

Command::Command(CommandType type, int recipeId, float processTime, float temperature)
	:type(type),
	recipeId(recipeId),
	processTime(processTime),
	temperature(temperature),
	waferId(-1),
	retryCount(0)
{
}

Command::Command(CommandType type, int waferId)
	:type(type),
	waferId(waferId),
	recipeId(-1),
	processTime(-1),
	temperature(-1),
	retryCount(0)
{
}

CommandType Command::GetCommandType() const {
	return type;
}

int Command::GetCommandRecipeId() const {
	return recipeId;
}

int Command::GetCommandWaferId() const {
	return waferId;
}

float Command::GetCommandProcessTime() const {
	return processTime;
}

float Command::GetCommandTemperature() const {
	return temperature;
}

std::string CommandTypeToString(CommandType type)
{
	switch (type)
	{
	case CommandType::None:
		return "None";
	case CommandType::Initialize:
		return "Initialize";
	case CommandType::SetRecipe:
		return "SetRecipe";
	case CommandType::CompleteInitialization:
		return "CompleteInitialization";
	case CommandType::LoadWafer:
		return "LoadWafer";
	case CommandType::Start:
		return "Start";
	case CommandType::Complete:
		return "Complete";
	case CommandType::RaiseError:
		return "RaiseError";
	case CommandType::Reset:
		return "Reset";
	case CommandType::PrintState:
		return "PrintState";
	}

	return "Unknown";
}

void Command::PrintCommand() const {
	std::cout << "CommandType : " << CommandTypeToString(type) << "\n";
}

void Command::IncreaseRetryCount() {
	retryCount++;
}

int Command::GetRetryCount() const{
	return retryCount;
}