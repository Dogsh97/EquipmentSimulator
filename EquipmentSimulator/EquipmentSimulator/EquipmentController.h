#pragma once

enum class EquipmentState{
	IDLE,
	INITIALIZING,
	READY,
	RUNNING,
	ERROR
};

class EquipmentController {
private:
	EquipmentState currentState;

public:
	EquipmentController();

	void Initialize();
	void CompleteInitialization();
	void Start();
	void Complete();
	void RaiseError();
	void Reset();
	void PrintState();
};