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
	bool ExecuteCommand(Command command);
	void CommandResult(Command command);
	void AddEventLog(Command command, bool success, CommandResultType type);

public:
	EquipmentController();

	void MakeCommand(Command command);
	void RunCommand();
	void PrintFailedCommands();
	void RetryFailedCommands();
	bool Initialize();
	bool SetRecipe(int id, float time, float temperature);
	bool CompleteInitialization();
	bool LoadWafer(int id);
	bool Start();
	bool Complete();
	bool RaiseError();
	bool Reset();
	bool PrintState();
	void PrintEventLogs();
	void PrintAlarmHistory();		
};