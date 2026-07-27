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

void EquipmentController::Start() {
	if (currentState == EquipmentState::READY) {
		currentState = EquipmentState::RUNNING;
	}
	else {
		std::cout << "[ERROR] Start command rejected.\n";
	}
}

void EquipmentController::Complete() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::READY;
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
			std::cout << "IDLE\n";
			break;
		case EquipmentState::INITIALIZING:
			std::cout << "INITIALIZING\n";
			break;
		case EquipmentState::READY:
			std::cout << "READY\n";
			break;
		case EquipmentState::RUNNING :
			std::cout << "RUNNING\n";
			break;
		case EquipmentState::ERROR:
			std::cout << "ERROR\n";
			break;
	}
}