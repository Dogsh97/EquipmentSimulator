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
		currentState = EquipmentState::Loading;
	}
	else {
		std::cout << "[ERROR] LoadWafer command rejected.\n";
	}
}

void EquipmentController::Start() {
	if (currentState == EquipmentState::Loading) {
		currentState = EquipmentState::RUNNING;
		wafer.StartProcessing();
	}
	else {
		std::cout << "[ERROR] Start command rejected.\n";
	}
}

void EquipmentController::Complete() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::READY;
		wafer.CompleteProcess();
	}
	else {
		std::cout << "[ERROR] Complete command rejected.\n";
	}
}

void EquipmentController::RaiseError() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::ERROR;
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
			std::cout << "IDLE\n";
			break;
		case EquipmentState::INITIALIZING:
			wafer.PrintInfo();
			std::cout << "INITIALIZING\n";
			break;
		case EquipmentState::Loading:
			wafer.PrintInfo();
			std::cout << "Loading\n";
			break;
		case EquipmentState::READY:
			wafer.PrintInfo();
			std::cout << "READY\n";
			break;
		case EquipmentState::RUNNING :
			wafer.PrintInfo();
			std::cout << "RUNNING\n";
			break;
		case EquipmentState::ERROR:
			wafer.PrintInfo();
			std::cout << "ERROR\n";
			break;
	}
}