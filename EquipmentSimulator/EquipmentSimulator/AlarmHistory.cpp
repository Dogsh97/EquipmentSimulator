#include <iostream>
#include "AlarmHistory.h"

AlarmHistory::AlarmHistory(AlarmCode alarmcode) 
	:alarmCode(alarmcode)
{
}

AlarmCode AlarmHistory::GetAlarmCode() const{
	return alarmCode;
}

void AlarmHistory::Print() const{
	std::cout << "===== AlarmHistory =====\n";
	std::cout << "Code : " << AlarmCodeToString(alarmCode) << "\n";	
}