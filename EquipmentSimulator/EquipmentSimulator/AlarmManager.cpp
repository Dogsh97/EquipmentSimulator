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
	std::cout << "===== Alarm =====\n";
	std::cout << "Code : " << AlarmCodeToString(currentAlarm) << "\n";
	
}

void AlarmManager::AddAlarmHistory(AlarmHistory alarmhistory) {
	alarmHistories.push_back(alarmhistory);
}

void AlarmManager::PrintAlarmHistory() const{
	for (int i = 0; i < alarmHistories.size(); ++i) {
		alarmHistories[i].Print();
	}
}