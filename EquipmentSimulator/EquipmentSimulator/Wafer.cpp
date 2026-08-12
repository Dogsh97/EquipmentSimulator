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

void Wafer::PrintInfo() {
	switch (state) {
		case WaferState::EMPTY:
			std::cout << "ID : " << waferId << "\n";
			std::cout << "State : EMPTY\n";
			break;
		case WaferState::LOADED:
			std::cout << "ID : " << waferId << "\n";
			std::cout << "State : LOADED\n";
			break;
		case WaferState::PROCESSING:
			std::cout << "ID : " << waferId << "\n";
			std::cout << "State : PROCESSING\n";
			break;
		case WaferState::COMPLETED:
			std::cout << "ID : " << waferId << "\n";
			std::cout << "State : COMPLETED\n";
			break;
	}
}	

WaferState Wafer::GetWaferState() {
	return state;
}