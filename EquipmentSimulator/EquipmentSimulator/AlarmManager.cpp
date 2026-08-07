#include <iostream>
#include "AlarmManager.h"

AlarmManager::AlarmManager()
	: currentAlarm(AlarmCode::NONE),
	alarmDetected(false)
{
}

void AlarmManager::RaiseAlarm(AlarmCode code) {
		alarmDetected = true;
		currentAlarm = code;
		AlarmHistory history(code);
		AddAlarmHistory(history);
}


void AlarmManager::ClearAlarm() {
	alarmDetected = false;
	currentAlarm = AlarmCode::NONE;
}

bool AlarmManager::HasAlarm() const {
	return alarmDetected;
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

void AlarmManager::AddAlarmHistory(AlarmHistory alarmhistory) {
	alarmHistories.push_back(alarmhistory);
}

void AlarmManager::PrintAlarmHistory() {
	for (int i = 0; i < alarmHistories.size(); ++i) {
		alarmHistories[i].Print();
	}
}