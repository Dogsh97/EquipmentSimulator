#include <iostream>
#include "Alarm.h"

AlarmManager::AlarmManager()
	: currentAlarm(AlarmCode::NONE),
	AlarmDetected(false)
{
}

void AlarmManager::RaiseAlarm(AlarmCode code) {
		AlarmDetected = true;
		currentAlarm = code;
}


void AlarmManager::ClearAlarm() {
	AlarmDetected = false;
	currentAlarm = AlarmCode::NONE;
}

bool AlarmManager::HasAlarm() const {
	return AlarmDetected;
}

void AlarmManager::PrintAlarm() const {
	switch (currentAlarm) {
		case AlarmCode::NONE:
			std::cout << "AlarmCode : NONE\n";
			break;
		case AlarmCode::EQUIPMENT_NOT_READY:
			std::cout << "AlarmCode : EQUIPMENT_NOT_READY\n";
			break;
		case AlarmCode::WAFER_NOT_DETECTED:
			std::cout << "AlarmCode : WAFER_NOT_DETECTED\n";
			break;
		case AlarmCode::PROCESS_ALREADY_RUNNING:
			std::cout << "AlarmCode : PROCESS_ALREADY_RUNNING\n";
			break;
		case AlarmCode::RECIPE_NOT_SET:
			std::cout << "AlarmCode : RECIPE_NOT_SET\n";
			break;
	}
	
}