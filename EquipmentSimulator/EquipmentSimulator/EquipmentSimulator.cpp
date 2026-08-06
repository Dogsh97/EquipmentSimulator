#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

	equipment.MakeCommand(Command(CommandType::LoadWafer, 1001));
	equipment.MakeCommand(Command(CommandType::Start));
	equipment.MakeCommand(Command(CommandType::Complete));
	equipment.MakeCommand(Command(CommandType::Reset));
	equipment.RunCommand();
	equipment.MakeCommand(Command(CommandType::Initialize));
	equipment.MakeCommand(Command(CommandType::CompleteInitialization));
	equipment.MakeCommand(Command(CommandType::SetRecipe,1,30,200));
	equipment.MakeCommand(Command(CommandType::LoadWafer,1001));
	equipment.MakeCommand(Command(CommandType::Start));
	equipment.MakeCommand(Command(CommandType::Complete));
	equipment.MakeCommand(Command(CommandType::PrintState));
	equipment.RunCommand();
	equipment.PrintFailedCommands();
	equipment.PrintEventLogs();
	return 0;
}
