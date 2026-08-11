#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

    equipment.MakeCommand(
        Command(CommandType::Initialize)
    );

    // INITIALIZING -> READY
    equipment.MakeCommand(
        Command(CommandType::CompleteInitialization)
    );

    equipment.MakeCommand(Command(CommandType::SetRecipe, -1, 10.0f, 200.0f));
    equipment.RunCommand();

    equipment.MakeCommand(Command(CommandType::SetRecipe, 1, 10.0f, 200.0f));

    equipment.MakeCommand(Command(CommandType::LoadWafer, -1));
    equipment.RunCommand();

    equipment.PrintEventLogs();
    equipment.PrintFailedCommands();

    return 0;
}

