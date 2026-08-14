#pragma once
#include "Wafer.h"
#include "Sensor.h"
#include "AlarmManager.h"
#include "Logger.h"
#include "Recipe.h"
#include "Command.h"
#include "CommandQueue.h"
#include "EventLog.h"
#include <queue>

enum class EquipmentState{
	IDLE,
	INITIALIZING,
	Loading,
	READY,
	RUNNING,
	ERROR
};

class EquipmentController {
private:
	EquipmentState currentState;
	Wafer wafer;
	Sensor sensor;
	AlarmManager alarmManager;
	Logger logger;
	Recipe recipe;
	CommandQueue commandQueue;
	std::queue<Command> failedCommandQueue;
	const int RetryCountMax = 3;
	bool CanExecute(Command command, EquipmentState state);
	bool CommandParameterValidation(Command command);
	bool InterlockValidation(Command command);
	bool PostValidation(Command command);
	void ExecuteCommand(Command command);
	void ProcessCommandResult(Command command);
	void AddEventLog(Command command, bool success, CommandResultType type);

public:
	EquipmentController();

	void MakeCommand(Command command);
	void RunCommand();
	void PrintFailedCommands();
	void RetryFailedCommands();
	void Initialize();
	void SetRecipe(int id, float time, float temperature);
	void CompleteInitialization();
	void LoadWafer(int id);
	void Start();
	void Complete();
	void RaiseError();
	void Reset();
	void PrintState();
	void PrintEventLogs();
	void PrintAlarmHistory();		
};