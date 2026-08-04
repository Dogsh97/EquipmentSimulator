#pragma once
#include "Wafer.h"
#include "Sensor.h"
#include "Alarm.h"
#include "Logger.h"
#include "Recipe.h"
#include "Command.h"
#include "CommandQueue.h"

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

public:
	EquipmentController();

	void MakeCommand(Command command);
	void RunCommand();
	void Initialize();
	void SetRecipe(int id, float time, float temperature);
	void CompleteInitialization();
	void LoadWafer(int id);
	void Start();
	void Complete();
	void RaiseError();
	void Reset();
	void PrintState();
};