#pragma once

enum class CommandType{
	None,
	Initialize,
	SetRecipe,
	CompleteInitialization,
	LoadWafer,
	Start,
	Complete,
	RaiseError,
	Reset,
	PrintState
};

class Command {
	private:
		CommandType type;
		int recipeId;
		float processTime;
		float temperature;
		int waferId;
		int retryCount;

	public:
		Command(CommandType type);
		Command(CommandType type, int recipeId, float processTime, float temperature);
		Command(CommandType type, int waferId);
		CommandType GetCommandType() const;
		int GetCommandRecipeId() const;
		int GetCommandWaferId() const;
		float GetCommandProcessTime() const;
		float GetCommandTemperature() const;
		void PrintCommand() const;
		void IncreaseRetryCount();
		int GetRetryCount() const;
		void ResetRetryCount();
};