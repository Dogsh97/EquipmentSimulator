#include <iostream>
#include "AlarmHistory.h"

AlarmHistory::AlarmHistory(AlarmCode alarmcode) 
	:alarmCode(alarmcode)
{
}

AlarmCode AlarmHistory::GetAlarmCode() {
	return alarmCode;
}

void AlarmHistory::Print() {
	switch (alarmCode) {
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