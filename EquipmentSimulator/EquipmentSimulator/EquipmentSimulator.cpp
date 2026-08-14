#include <iostream>
#include "EquipmentController.h"

int main()
{
    // ==================================================
   // Test 1. Normal Flow
   // ==================================================

    {
        std::cout << "\n===== Test 1 : Normal Flow =====\n";

        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::Initialize));
        equipment.MakeCommand(Command(CommandType::CompleteInitialization));
        equipment.MakeCommand(Command(CommandType::SetRecipe, 1, 1, 1));
        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
        equipment.MakeCommand(Command(CommandType::Start));
        equipment.MakeCommand(Command(CommandType::Complete));

        equipment.RunCommand();

        equipment.PrintEventLogs();
    }


    // ==================================================
    // Test 2. CanExecute Failed
    // ==================================================

    {
        std::cout << "\n===== Test 2 : CanExecute Failed =====\n";

        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::Start));

        equipment.RunCommand();

        equipment.PrintEventLogs();
    }


    // ==================================================
    // Test 3. Parameter Validation Failed
    // ==================================================

    {
        std::cout << "\n===== Test 3 : Parameter Validation Failed =====\n";

        EquipmentController equipment;

        equipment.MakeCommand(
            Command(CommandType::SetRecipe, -11, 1, 1)
        );

        equipment.RunCommand();

        equipment.PrintEventLogs();
    }


    // ==================================================
    // Test 4. CanExecute Failed (RECIPE_NOT_SET)
    // ==================================================

    {
        std::cout << "\n===== Test 4 : CanExecute Failed (RECIPE_NOT_SET) =====\n";

        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::Initialize));
        equipment.MakeCommand(Command(CommandType::CompleteInitialization));
        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
        equipment.MakeCommand(Command(CommandType::Start));

        equipment.RunCommand();

        equipment.PrintEventLogs();
    }


    // ==================================================
    // Test 5. Process Already Running
    // ==================================================

    {
        std::cout << "\n===== Test 5 : Process Already Running =====\n";

        EquipmentController equipment;

        equipment.MakeCommand(Command(CommandType::Initialize));
        equipment.MakeCommand(Command(CommandType::CompleteInitialization));
        equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
        equipment.MakeCommand(Command(CommandType::Start));
        equipment.MakeCommand(Command(CommandType::Start));

        equipment.RunCommand();

        equipment.PrintEventLogs();
    }

    std::cout << "\n===== Test 6 : Reset State Consistency =====\n";

    EquipmentController equipment;

    equipment.MakeCommand(Command(CommandType::Initialize));
    equipment.MakeCommand(Command(CommandType::CompleteInitialization));
    equipment.MakeCommand(Command(CommandType::SetRecipe, 1, 1, 1));
    equipment.MakeCommand(Command(CommandType::LoadWafer, 1));
    equipment.MakeCommand(Command(CommandType::Start));
    equipment.MakeCommand(Command(CommandType::RaiseError));
    equipment.MakeCommand(Command(CommandType::Reset));

    equipment.RunCommand();

    equipment.PrintState();
    equipment.PrintEventLogs();

    return 0;

}

