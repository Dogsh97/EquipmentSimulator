#pragma once
#include "AlarmHistory.h"
#include "AlarmCode.h"
#include <iostream>
#include <vector>
class AlarmManager {
	private:
		AlarmCode currentAlarm;
		bool alarmDetected;
		std::vector<AlarmHistory> alarmHistories;

	public:
		AlarmManager();
		void RaiseAlarm(AlarmCode code);
		void ClearAlarm();
		bool HasAlarm() const;
		void PrintAlarm() const;
		void AddAlarmHistory(AlarmHistory alarmhistory);
		void PrintAlarmHistory();
};