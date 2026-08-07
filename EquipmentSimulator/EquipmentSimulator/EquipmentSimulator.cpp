#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

	equipment.RunCommand();

	equipment.PrintFailedCommands();
	equipment.PrintEventLogs();
	equipment.PrintAlarmHistory();

	std::cout << "\n===== Retry =====\n";

	equipment.RetryFailedCommands();

	equipment.PrintFailedCommands();
	equipment.PrintEventLogs();

	return 0;
}
