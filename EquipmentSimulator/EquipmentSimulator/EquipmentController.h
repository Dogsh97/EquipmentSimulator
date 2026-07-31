#pragma once
#include "Wafer.h"
#include "Sensor.h"
#include "Alarm.h"
#include "Logger.h"

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

public:
	EquipmentController();

	void Initialize();
	void CompleteInitialization();
	void LoadWafer(int id);
	void Start();
	void Complete();
	void RaiseError();
	void Reset();
	void PrintState();
};