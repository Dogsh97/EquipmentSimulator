#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

    // 1. IDLE -> INITIALIZING
    equipment.MakeCommand(
        Command(CommandType::Initialize)
    );

    // 2. INITIALIZING -> READY
    equipment.MakeCommand(
        Command(CommandType::CompleteInitialization)
    );

    // 3. READY -> Recipe 설정
    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, 200.0f)
    );

    // 4. READY -> Loading
    // Wafer : LOADED
    // Sensor : detected
    // Alarm : false
    equipment.MakeCommand(
        Command(CommandType::LoadWafer, 1)
    );

    // 5. Loading -> RUNNING
    // Interlock:
    // Wafer = LOADED
    // Sensor = detected
    // Alarm = false
    equipment.MakeCommand(
        Command(CommandType::Start)
    );

    // 6. RUNNING -> ERROR
    // Interlock:
    // Wafer = PROCESSING
    // Sensor = detected
    // Alarm = false
    equipment.MakeCommand(
        Command(CommandType::RaiseError)
    );

    // 7. ERROR -> READY
    // Interlock:
    // Wafer = PROCESSING
    // Sensor = detected
    // Alarm = true

    equipment.MakeCommand(
        Command(CommandType::Start)
    );


    equipment.MakeCommand(
        Command(CommandType::Reset)
    );

    equipment.RunCommand();

    std::cout << "\n===== Event Logs =====\n";
    equipment.PrintEventLogs();

    std::cout << "\n===== Failed Commands =====\n";
    equipment.PrintFailedCommands();

    return 0;
}

