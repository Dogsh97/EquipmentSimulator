#include <iostream>
#include "EquipmentController.h"

int main()
{
    EquipmentController equipment;

    equipment.MakeCommand(Command(CommandType::Initialize));
    equipment.MakeCommand(Command(CommandType::CompleteInitialization));

    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, 200.0f)
    );

    equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
    equipment.MakeCommand(Command(CommandType::Start));
    equipment.MakeCommand(Command(CommandType::Complete));

    equipment.MakeCommand(Command(CommandType::LoadWafer, 2));
    equipment.MakeCommand(Command(CommandType::Start));
    equipment.MakeCommand(Command(CommandType::RaiseError));
    equipment.MakeCommand(Command(CommandType::Reset));

    equipment.RunCommand();

    std::cout << "\n===== Event Logs =====\n";
    equipment.PrintEventLogs();

    std::cout << "\n===== Failed Commands =====\n";
    equipment.PrintFailedCommands();

    return 0;
}

