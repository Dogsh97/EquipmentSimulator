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
        equipment.MakeCommand(Command(CommandType::Complete));

        equipment.RunCommand();

        std::cout << "\n--- Failed Command Queue ---\n";
        equipment.PrintFailedCommands();

        std::cout << "\n--- Event Logs ---\n";
        equipment.PrintEventLogs();
    }

    
    std::cout << "\n===== Test 2 : Error Recovery + WAFER_ALREADY =====\n";

    {
        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::Initialize));
        equipment.MakeCommand(Command(CommandType::CompleteInitialization));
        equipment.MakeCommand(Command(CommandType::SetRecipe, 1, 1, 1));
        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
        equipment.MakeCommand(Command(CommandType::Start));

        equipment.MakeCommand(Command(CommandType::RaiseError));
        equipment.MakeCommand(Command(CommandType::Reset));

        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));

        equipment.MakeCommand(Command(CommandType::Start));
        equipment.MakeCommand(Command(CommandType::Complete));

        equipment.RunCommand();

        std::cout << "\n--- Failed Command Queue ---\n";
        equipment.PrintFailedCommands();

        std::cout << "\n--- Event Logs ---\n";
        equipment.PrintEventLogs();
    }

   
    std::cout << "\n===== Test 3 : Validation Failed =====\n";

    {
        EquipmentController equipment;

        equipment.MakeCommand(
            Command(CommandType::CompleteInitialization)
        );

        equipment.RunCommand();

        std::cout << "\n--- Failed Command Queue ---\n";
        equipment.PrintFailedCommands();

        std::cout << "\n--- Event Logs ---\n";
        equipment.PrintEventLogs();
    }

    
    std::cout << "\n===== Test 4 : Retry Failed =====\n";

    {
        EquipmentController equipment;

        equipment.TestAddFailedCommand(
            Command(CommandType::Start)
        );

        std::cout << "\n--- Before Retry ---\n";
        equipment.PrintFailedCommands();

        equipment.RetryFailedCommands();

        std::cout << "\n--- After Retry ---\n";
        equipment.PrintFailedCommands();

        std::cout << "\n--- Event Logs ---\n";
        equipment.PrintEventLogs();
    }
    
    return 0;

}

