#include <iostream>
#include "EquipmentController.h"

int main()
{
    EquipmentController equipment;

    std::cout << "\n===== Test 1 : CanExecute Failed =====\n";

    equipment.MakeCommand(Command(CommandType::CompleteInitialization));
    equipment.RunCommand();

    equipment.PrintFailedCommands();
    equipment.PrintEventLogs();
    equipment.ResetEventLogs();


    std::cout << "\n===== Test 2 : Normal Flow =====\n";

    equipment.MakeCommand(Command(CommandType::Initialize));
    equipment.MakeCommand(Command(CommandType::CompleteInitialization));
    equipment.MakeCommand(Command(CommandType::SetRecipe,1,1,1));
    equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
    equipment.MakeCommand(Command(CommandType::Start));

    equipment.RunCommand();

    equipment.PrintFailedCommands();
    equipment.PrintEventLogs();
    equipment.ResetEventLogs();


    std::cout << "\n===== Test 3 : Retry Failed Command =====\n";

    equipment.MakeCommand(Command(CommandType::Complete));
    equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
    equipment.RunCommand();

    //equipment.TestAddFailedCommand(Command(CommandType::Start));

    equipment.PrintFailedCommands();

    equipment.RetryFailedCommands();

    equipment.PrintFailedCommands();
    equipment.PrintEventLogs();
    equipment.ResetEventLogs();

    std::cout << "\n===== Test 4 : Retry Failed =====\n";

    equipment.MakeCommand(Command(CommandType::Start));
    equipment.RunCommand();

    //equipment.TestAddFailedCommand(Command(CommandType::Start));

    equipment.PrintFailedCommands();

    equipment.RetryFailedCommands();

    equipment.PrintFailedCommands();
    equipment.PrintEventLogs();
    equipment.ResetEventLogs();

    return 0;

}

