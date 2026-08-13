#include <iostream>
#include "EquipmentController.h"

int main()
{
    EquipmentController equipment;

    equipment.MakeCommand(Command(CommandType::Initialize));
    equipment.MakeCommand(Command(CommandType::CompleteInitialization));
    equipment.MakeCommand(Command(CommandType::SetRecipe,1, 1, 1));
    equipment.MakeCommand(Command(CommandType::LoadWafer,1));
    equipment.MakeCommand(Command(CommandType::Start));
    equipment.MakeCommand(Command(CommandType::Complete));
    equipment.RunCommand();

    equipment.PrintState();
    
    equipment.PrintEventLogs();


    equipment.PrintFailedCommands();

    return 0;

}

