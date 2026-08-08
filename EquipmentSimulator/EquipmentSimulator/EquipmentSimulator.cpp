#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

	// 장비 초기화
	equipment.MakeCommand(Command(CommandType::Initialize));
	equipment.MakeCommand(Command(CommandType::CompleteInitialization));
	equipment.RunCommand();

	// Retry가 계속 실패하도록 Command 추가
	equipment.MakeCommand(Command(CommandType::Start));
	equipment.RunCommand();

	// 최초 실패 확인
	std::cout << "\n===== Initial Failed Command =====\n";
	equipment.PrintFailedCommands();

	// Retry 1
	std::cout << "\n===== Retry 1 =====\n";
	equipment.RetryFailedCommands();
	equipment.PrintFailedCommands();

	// Retry 2
	std::cout << "\n===== Retry 2 =====\n";
	equipment.RetryFailedCommands();
	equipment.PrintFailedCommands();

	// Retry 3
	std::cout << "\n===== Retry 3 =====\n";
	equipment.RetryFailedCommands();
	equipment.PrintFailedCommands();

	// Retry 4 - 제한 초과 확인
	std::cout << "\n===== Retry 4 =====\n";
	equipment.RetryFailedCommands();
	equipment.PrintFailedCommands();
	return 0;
}
