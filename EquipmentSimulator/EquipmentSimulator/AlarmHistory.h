#pragma once
#include "AlarmCode.h"
#include <iostream>

class AlarmHistory {
	private:
		AlarmCode alarmCode;
	public:
		AlarmHistory(AlarmCode alarmcode);

		AlarmCode GetAlarmCode();
		void Print();
};