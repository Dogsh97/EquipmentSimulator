#include <iostream>
#include "EquipmentController.h"

int main()
{
    EquipmentController equipment;

    std::cout << "\n===== Test 1 : Normal Flow =====\n";

    {
        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::Initialize));
        equipment.MakeCommand(Command(CommandType::CompleteInitialization));
        equipment.MakeCommand(Command(CommandType::SetRecipe, 1, 1, 1));
        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
        equipment.MakeCommand(Command(CommandType::Start));

        equipment.RunCommand();

        equipment.PrintFailedCommands();
        equipment.PrintEventLogs();
    }


    std::cout << "\n===== Test 2 : Validation Failed =====\n";

    {
        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::CompleteInitialization));

        equipment.RunCommand();

        equipment.PrintFailedCommands();
        equipment.PrintEventLogs();
    }


    std::cout << "\n===== Test 3 : Retry Failed =====\n";

    {
        EquipmentController equipment;

        equipment.TestAddFailedCommand(Command(CommandType::Start));

        std::cout << "\n--- Before Retry ---\n";
        equipment.PrintFailedCommands();

        equipment.RetryFailedCommands();

        std::cout << "\n--- After Retry ---\n";
        equipment.PrintFailedCommands();

        equipment.PrintEventLogs();
    }

    return 0;

}

