#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;


    // ========================================
   // Test 1. 잘못된 State에서 SetRecipe 실행
   // ========================================
    std::cout << "===== Test 1 : Invalid State =====\n";

    // 현재 상태 : IDLE
    // SetRecipe는 READY에서만 실행 가능
    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, 200.0f)
    );

    equipment.RunCommand();


    // ========================================
    // Test 2. 정상적인 State에서 SetRecipe 성공
    // ========================================
    std::cout << "\n===== Test 2 : Valid SetRecipe =====\n";

    // IDLE -> INITIALIZING
    equipment.MakeCommand(
        Command(CommandType::Initialize)
    );

    // INITIALIZING -> READY
    equipment.MakeCommand(
        Command(CommandType::CompleteInitialization)
    );

    // READY -> SetRecipe 성공
    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, 200.0f)
    );

    equipment.RunCommand();


    // ========================================
    // Test 3. 정상 State지만 잘못된 Recipe
    // ========================================
    std::cout << "\n===== Test 3 : Invalid Recipe =====\n";

    // 현재 상태 : READY
    // State Validation은 통과하지만
    // Recipe Validation에서 실패해야 함
    equipment.MakeCommand(
        Command(CommandType::SetRecipe, 1, 10.0f, -1.0f)
    );

    equipment.RunCommand();


    // ========================================
    // Failed Command 확인
    // ========================================
    std::cout << "\n===== Failed Commands =====\n";

    equipment.PrintFailedCommands();


    // ========================================
    // Event Log 확인
    // ========================================
    std::cout << "\n===== Event Logs =====\n";

    equipment.PrintEventLogs();


    return 0;
}

