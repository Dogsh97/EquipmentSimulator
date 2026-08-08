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

void Command::PrintCommand() const {
	switch (type) {
		case CommandType::None:
			std::cout << "CommandType : None";
			break;

		case CommandType::Initialize:
			std::cout << "CommandType : Initialize";
			break;

		case CommandType::SetRecipe:
			std::cout << "CommandType : SetRecipe";
			break;

		case CommandType::CompleteInitialization:
			std::cout << "CommandType : CompleteInitialization";
			break;

		case CommandType::LoadWafer:
			std::cout << "CommandType : LoadWafer";
			break;

		case CommandType::Start:
			std::cout << "CommandType : Start";
			break;

		case CommandType::Complete:
			std::cout << "CommandType : Complete";
			break;

		case CommandType::RaiseError:
			std::cout << "CommandType : RaiseError";
			break;

		case CommandType::Reset:
			std::cout << "CommandType : Reset";
			break;

		case CommandType::PrintState:
			std::cout << "CommandType : PrintState";
			break;
	}
}

void Command::IncreaseRetryCount() {
	retryCount++;
}

int Command::GetRetryCount() const{
	return retryCount;
}

void Command::ResetRetryCount() {
	retryCount = 0;
}