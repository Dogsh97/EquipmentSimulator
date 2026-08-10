#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

    std::cout << "===== Valid SetRecipe Command =====\n";

    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, 200.0f)
    );

    equipment.RunCommand();

    std::cout << "\n===== Invalid SetRecipe Command =====\n";

    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, -1.0f)
    );

    equipment.RunCommand();

    std::cout << "\n===== Failed Commands =====\n";

    equipment.PrintFailedCommands();

    std::cout << "\n===== Event Logs =====\n";

    equipment.PrintEventLogs();


	return 0;
}
