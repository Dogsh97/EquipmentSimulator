#pragma once
#include "AlarmCode.h"

class AlarmHistory {
	private:
		AlarmCode alarmCode;
	public:
		AlarmHistory(AlarmCode alarmcode);
		AlarmCode GetAlarmCode() const;
		void Print() const;
};