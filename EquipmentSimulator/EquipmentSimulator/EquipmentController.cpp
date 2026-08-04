#include <iostream>
#include "EquipmentController.h"
#include <string>

EquipmentController::EquipmentController() 
	: currentState(EquipmentState::IDLE),
	command(CommandType::None)
{
}

void EquipmentController::MakeCommand(Command command) {
	commandQueue.PushCommand(command);
}

void EquipmentController::RunCommand() {
	while(commandQueue.CommandDetected()) {
		command = commandQueue.GetCommand();
		switch (command.GetCommandType()) {
			case CommandType::Initialize:
				Initialize();
				break;

			case CommandType::SetRecipe:
				SetRecipe(command.GetCommandRecipeId(), command.GetCommandProcessTime(), command.GetCommandTemperature());
				break;

			case CommandType::CompleteInitialization:
				CompleteInitialization();
				break;

			case CommandType::LoadWafer:
				LoadWafer(command.GetCommandWaferId());
				break;

			case CommandType::Start:
				Start();
				break;

			case CommandType::Complete:
				Complete();
				break;

			case CommandType::RaiseError:
				RaiseError();
				break;

			case CommandType::Reset:
				Reset();
				break;

			case CommandType::PrintState:
				PrintState();
				break;
		}
		commandQueue.PopCommand();
	}
}

void EquipmentController::Initialize() {
	if (currentState == EquipmentState::IDLE) {
		currentState = EquipmentState::INITIALIZING;
		logger.Log("INITIALIZE");
	}
	else {
		std::cout << "[ERROR] Initialize command rejected.\n";
	}
}

void EquipmentController::SetRecipe(int id, float time, float temperature) {
	recipe.SetRecipe(id, time, temperature);
	logger.Log("SetRecipe");
}

void EquipmentController::CompleteInitialization() {
	if (currentState == EquipmentState::INITIALIZING) {
		currentState = EquipmentState::READY;
		logger.Log("CompleteInitialization");
		
	}
	else {
		std::cout << "[ERROR] CompleteInitialization command rejected.\n";
	}
}

void EquipmentController::LoadWafer(int id) {
	if (currentState == EquipmentState::READY) {
		wafer.Load(id);
		sensor.DetectWafer();
		currentState = EquipmentState::Loading;
		alarmManager.ClearAlarm();
		logger.Log("LoadWafer id :" + std::to_string(id));
	}
	else {
		std::cout << "[ERROR] LoadWafer command rejected.\n";
		alarmManager.RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
		alarmManager.PrintAlarm();
	}
}

void EquipmentController::Start() {
	if (sensor.IsDetected()) {
		if (currentState == EquipmentState::Loading) {
			if (recipe.IsSetting()) {
				currentState = EquipmentState::RUNNING;
				wafer.StartProcessing();
				alarmManager.ClearAlarm();
				logger.Log("Start");
			}
			else {
				std::cout << "[ERROR] Recipe Not Setting.\n";
				alarmManager.RaiseAlarm(AlarmCode::RECIPE_NOT_SET);
				alarmManager.PrintAlarm();
			}
		}
		else {
			std::cout << "[ERROR] Start command rejected.\n";
			alarmManager.RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
			alarmManager.PrintAlarm();
		}
	}
	else {
		alarmManager.RaiseAlarm(AlarmCode::WAFER_NOT_DETECTED);
		alarmManager.PrintAlarm();
	}
}

void EquipmentController::Complete() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::READY;
		wafer.CompleteProcess();
		sensor.RemoveWafer();
		logger.Log("Complete");
	}
	else {
		std::cout << "[ERROR] Complete command rejected.\n";
	}
}

void EquipmentController::RaiseError() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::ERROR;
		alarmManager.RaiseAlarm(AlarmCode::PROCESS_ALREADY_RUNNING);
		alarmManager.PrintAlarm();
		logger.Log("RaiseError");
	}
	else {
		std::cout << "[ERROR] RaiseError command rejected.\n";
	}
}

void EquipmentController::Reset() {
	if (currentState == EquipmentState::ERROR) {
		currentState = EquipmentState::READY;
		logger.Log("Reset");
	}
	else {
		std::cout << "[ERROR] Reset command rejected.\n";
	}
}

void EquipmentController::PrintState() {
	switch (currentState) {
		case EquipmentState::IDLE:
			wafer.PrintInfo();
			sensor.PrintStatus();
			alarmManager.PrintAlarm();
			logger.PrintLog();
			std::cout << "IDLE\n";
			break;
		case EquipmentState::INITIALIZING:
			wafer.PrintInfo();
			sensor.PrintStatus();
			alarmManager.PrintAlarm();
			logger.PrintLog();
			std::cout << "INITIALIZING\n";
			break;
		case EquipmentState::Loading:
			wafer.PrintInfo();
			sensor.PrintStatus();
			alarmManager.PrintAlarm();
			logger.PrintLog();
			std::cout << "Loading\n";
			break;
		case EquipmentState::READY:
			wafer.PrintInfo();
			sensor.PrintStatus();
			alarmManager.PrintAlarm();
			logger.PrintLog();
			std::cout << "READY\n";
			break;
		case EquipmentState::RUNNING :
			wafer.PrintInfo();
			sensor.PrintStatus();
			alarmManager.PrintAlarm();
			logger.PrintLog();
			std::cout << "RUNNING\n";
			break;
		case EquipmentState::ERROR:
			wafer.PrintInfo();
			sensor.PrintStatus();
			alarmManager.PrintAlarm();
			logger.PrintLog();
			std::cout << "ERROR\n";
			break;
	}
}