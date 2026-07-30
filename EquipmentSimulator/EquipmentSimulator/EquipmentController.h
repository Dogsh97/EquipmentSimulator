#pragma once
#include "Wafer.h"
#include "Sensor.h"

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
	Sensor Sensor;

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