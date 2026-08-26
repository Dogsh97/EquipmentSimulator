#include "Wafer.h"
#include <iostream>
Wafer::Wafer()
	: waferId(-1),
	state(WaferState::EMPTY)
{
}

void Wafer::Load(int id) {
	if (state == WaferState::EMPTY) {
		waferId = id;
		state = WaferState::LOADED;
	}
	else {
		std::cout << "[ERROR] Cannot Load processing. Wafer is not Empty.";
	}
}

void Wafer::StartProcessing() {
	if (state == WaferState::LOADED) {
		state = WaferState::PROCESSING;
	}
	else {
		std::cout << "[ERROR] Cannot start processing. Wafer is not loaded.";
	}
}

void Wafer::CompleteProcess() {
	if (state == WaferState::PROCESSING) {
		state = WaferState::COMPLETED;
	}
	else {
		std::cout << "[ERROR] Cannot Complete process. Wafer is not Processing.";
	}
}

void Wafer::ResetProcess() {
	if (state == WaferState::PROCESSING || state == WaferState::COMPLETED) {
		state = WaferState::EMPTY;
	}
	else {
		std::cout << "[ERROR] Cannot Reset process. Wafer is not Completed.";
	}
}

std::string WaferStateToString(WaferState state) {
	switch (state) {
		case WaferState::EMPTY:
			return "EMPTY";
		case WaferState::LOADED:
			return "LOADED";
		case WaferState::PROCESSING:
			return "PROCESSING";
		case WaferState::COMPLETED:
			return "COMPLETED";
		}
	return "UNKNOWN";
}

void Wafer::PrintInfo() {
	std::cout << "ID : " << waferId << "\n";
	std::cout << "State: " << WaferStateToString(state) << "\n";
}	

WaferState Wafer::GetWaferState() const {
	return state; 
}