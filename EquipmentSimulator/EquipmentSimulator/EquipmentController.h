#pragma once
#include "Wafer.h"
#include "Sensor.h"
#include "Alarm.h"
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
	Command command;
	CommandQueue commandQueue;
	std::queue<Command> failedCommandQueue;

public:
	EquipmentController();

	void MakeCommand(Command command);
	void RunCommand();
	void PrintFailedCommands();
	bool Initialize();
	void SetRecipe(int id, float time, float temperature);
	bool CompleteInitialization();
	bool LoadWafer(int id);
	bool Start();
	bool Complete();
	bool RaiseError();
	bool Reset();
	void PrintState();
	void PrintEventLogs();
};