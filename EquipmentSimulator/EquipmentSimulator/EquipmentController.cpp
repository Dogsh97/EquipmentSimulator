#include <iostream>
#include "EquipmentController.h"

EquipmentController::EquipmentController() 
	: currentState(EquipmentState::IDLE)
{
}

void EquipmentController::Initialize() {
	if (currentState == EquipmentState::IDLE) {
		currentState = EquipmentState::INITIALIZING;
	}
	else {
		std::cout << "[ERROR] Initialize command rejected.\n";
	}
}

void EquipmentController::CompleteInitialization() {
	if (currentState == EquipmentState::INITIALIZING) {
		currentState = EquipmentState::READY;
		
	}
	else {
		std::cout << "[ERROR] CompleteInitialization command rejected.\n";
	}
}

void EquipmentController::LoadWafer(int id) {
	if (currentState == EquipmentState::READY) {
		wafer.Load(id);
		Sensor.DetectWafer();
		currentState = EquipmentState::Loading;
		AlarmManager.ClearAlarm();
	}
	else {
		std::cout << "[ERROR] LoadWafer command rejected.\n";
		AlarmManager.RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
		AlarmManager.PrintAlarm();
	}
}

void EquipmentController::Start() {
	if (Sensor.IsDetected()) {
		if (currentState == EquipmentState::Loading) {
			currentState = EquipmentState::RUNNING;
			wafer.StartProcessing();
			AlarmManager.ClearAlarm();
		}
		else {
			std::cout << "[ERROR] Start command rejected.\n";
			AlarmManager.RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
			AlarmManager.PrintAlarm();
		}
	}
	else {
		AlarmManager.RaiseAlarm(AlarmCode::WAFER_NOT_DETECTED);
		AlarmManager.PrintAlarm();
	}
}

void EquipmentController::Complete() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::READY;
		wafer.CompleteProcess();
		Sensor.RemoveWafer();
	}
	else {
		std::cout << "[ERROR] Complete command rejected.\n";
	}
}

void EquipmentController::RaiseError() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::ERROR;
		AlarmManager.RaiseAlarm(AlarmCode::PROCESS_ALREADY_RUNNING);
		AlarmManager.PrintAlarm();
	}
	else {
		std::cout << "[ERROR] RaiseError command rejected.\n";
	}
}

void EquipmentController::Reset() {
	if (currentState == EquipmentState::ERROR) {
		currentState = EquipmentState::READY;
	}
	else {
		std::cout << "[ERROR] Reset command rejected.\n";
	}
}

void EquipmentController::PrintState() {
	switch (currentState) {
		case EquipmentState::IDLE:
			wafer.PrintInfo();
			Sensor.PrintStatus();
			AlarmManager.PrintAlarm();
			std::cout << "IDLE\n";
			break;
		case EquipmentState::INITIALIZING:
			wafer.PrintInfo();
			Sensor.PrintStatus();
			AlarmManager.PrintAlarm();
			std::cout << "INITIALIZING\n";
			break;
		case EquipmentState::Loading:
			wafer.PrintInfo();
			Sensor.PrintStatus();
			AlarmManager.PrintAlarm();
			std::cout << "Loading\n";
			break;
		case EquipmentState::READY:
			wafer.PrintInfo();
			Sensor.PrintStatus();
			AlarmManager.PrintAlarm();
			std::cout << "READY\n";
			break;
		case EquipmentState::RUNNING :
			wafer.PrintInfo();
			Sensor.PrintStatus();
			AlarmManager.PrintAlarm();
			std::cout << "RUNNING\n";
			break;
		case EquipmentState::ERROR:
			wafer.PrintInfo();
			Sensor.PrintStatus();
			AlarmManager.PrintAlarm();
			std::cout << "ERROR\n";
			break;
	}
}