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
		if (CommandParameterValidation()) {
			if (CanExecute(currentState)) {
				switch (command.GetCommandType()) {
				case CommandType::Initialize:
				{
					bool isSuccess = Initialize();
					EventLog eventlog(CommandType::Initialize, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::SetRecipe:
				{
					bool isSuccess = SetRecipe(command.GetCommandRecipeId(), command.GetCommandProcessTime(), command.GetCommandTemperature());
					EventLog eventlog(CommandType::SetRecipe, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::CompleteInitialization:
				{
					bool isSuccess = CompleteInitialization();
					EventLog eventlog(CommandType::CompleteInitialization, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::LoadWafer:
				{
					bool isSuccess = LoadWafer(command.GetCommandWaferId());

					EventLog eventlog(CommandType::LoadWafer, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::Start:
				{
					bool isSuccess = Start();
					EventLog eventlog(CommandType::Start, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::Complete:
				{
					bool isSuccess = Complete();
					EventLog eventlog(CommandType::Complete, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::RaiseError:
				{
					bool isSuccess = RaiseError();
					EventLog eventlog(CommandType::RaiseError, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::Reset:
				{
					bool isSuccess = Reset();
					EventLog eventlog(CommandType::Reset, isSuccess);
					logger.AddEventLog(eventlog);
					if (!isSuccess) {
						failedCommandQueue.push(command);
					}
					break;
				}

				case CommandType::PrintState:
				{
					PrintState();
					EventLog eventlog(CommandType::PrintState, true);
					logger.AddEventLog(eventlog);
					break;
				}
				}
			}
			else {
				EventLog eventlog(command.GetCommandType(), false);
				logger.AddEventLog(eventlog);
			}
		}
		else {
			EventLog eventlog(command.GetCommandType(), false);
			logger.AddEventLog(eventlog);
		}
		commandQueue.PopCommand();
	}
}

void EquipmentController::PrintFailedCommands() {
	std::queue<Command> temp;
	temp = failedCommandQueue;
	while (!temp.empty()) {
		std::cout << "Command:";
		temp.front().PrintCommand();
		std::cout << "\n";
		temp.pop();
	}
}

void EquipmentController::RetryFailedCommands() {
		while (!failedCommandQueue.empty()) {
			Command temp(failedCommandQueue.front());
			if (temp.GetRetryCount() < RetryCountMax) {
				temp.IncreaseRetryCount();
				commandQueue.PushCommand(temp);
			}
			failedCommandQueue.pop();
		}
		RunCommand();
}

bool EquipmentController::CanExecute(EquipmentState state) {
	switch (command.GetCommandType()) {
		case CommandType::Initialize:
			if (state == EquipmentState::IDLE) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::CompleteInitialization:
			if (state == EquipmentState::INITIALIZING) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::SetRecipe:
			if (state == EquipmentState::READY) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::LoadWafer:
			if (state == EquipmentState::READY) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::Start:
			if (state == EquipmentState::Loading) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::Complete:
			if (state == EquipmentState::RUNNING) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::Reset:
			if (state == EquipmentState::ERROR) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::RaiseError:
			if (state == EquipmentState::RUNNING) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::PrintState:
			return true;
	}
	return false;
}

bool EquipmentController::CommandParameterValidation() {
	switch (command.GetCommandType()) {
	case CommandType::SetRecipe:
		if (command.GetCommandRecipeId() <= 0) {
			return false;
		}
		return true;

	case CommandType::LoadWafer:
		if (command.GetCommandWaferId() <= 0) {
			return false;
		}
		else {
			return true;
		}
	}
	return true;
	//변수가 없는 함수들은 모두 통과되도록 설계(확장된다면 case를 늘리는 방향으로 확장성 설계)
}

bool EquipmentController::Initialize() {
	if (currentState == EquipmentState::IDLE) {
		currentState = EquipmentState::INITIALIZING;
		logger.Log("INITIALIZE");
		return true;
	}
	else {
		std::cout << "[ERROR] Initialize command rejected.\n";
		return false;
	}
}

bool EquipmentController::SetRecipe(int id, float time, float temperature) {	
	if(!recipe.IsValid(time, temperature)) {
		logger.Log("Recipe is not Valid");
		return false;
	}
	else {
		recipe.SetRecipe(id, time, temperature);
		logger.Log("SetRecipe");
		return true;
	}	
}

bool EquipmentController::CompleteInitialization() {
	if (currentState == EquipmentState::INITIALIZING) {
		currentState = EquipmentState::READY;
		logger.Log("CompleteInitialization");
		return true;
		
	}
	else {
		std::cout << "[ERROR] CompleteInitialization command rejected.\n";
		return false;
	}
}

bool EquipmentController::LoadWafer(int id) {
	if (currentState == EquipmentState::READY) {
		wafer.Load(id);
		sensor.DetectWafer();
		currentState = EquipmentState::Loading;
		alarmManager.ClearAlarm();
		logger.Log("LoadWafer id :" + std::to_string(id));
		return true;
	}
	else {
		std::cout << "[ERROR] LoadWafer command rejected.\n";
		alarmManager.RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
		alarmManager.PrintAlarm();
		return false;
	}
}

bool EquipmentController::Start() {
	if (sensor.IsDetected()) {
		if (currentState == EquipmentState::Loading) {
			if (recipe.IsSetting()) {
				currentState = EquipmentState::RUNNING;
				wafer.StartProcessing();
				alarmManager.ClearAlarm();
				logger.Log("Start");
				return true;
			}
			else {
				std::cout << "[ERROR] Recipe Not Setting.\n";
				alarmManager.RaiseAlarm(AlarmCode::RECIPE_NOT_SET);
				alarmManager.PrintAlarm();
				return false;
			}
		}
		else {
			std::cout << "[ERROR] Start command rejected.\n";
			alarmManager.RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
			alarmManager.PrintAlarm();
			return false;
		}
	}
	else {
		alarmManager.RaiseAlarm(AlarmCode::WAFER_NOT_DETECTED);
		alarmManager.PrintAlarm();
		return false;
	}
}

bool EquipmentController::Complete() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::READY;
		wafer.CompleteProcess();
		sensor.RemoveWafer();
		logger.Log("Complete");
		return true;
	}
	else {
		std::cout << "[ERROR] Complete command rejected.\n";
		return false;
	}
}

bool EquipmentController::RaiseError() {
	if (currentState == EquipmentState::RUNNING) {
		currentState = EquipmentState::ERROR;
		alarmManager.RaiseAlarm(AlarmCode::PROCESS_ALREADY_RUNNING);
		alarmManager.PrintAlarm();
		logger.Log("RaiseError");
		return true;
	}
	else {
		std::cout << "[ERROR] RaiseError command rejected.\n";
		return false;
	}
}

bool EquipmentController::Reset() {
	if (currentState == EquipmentState::ERROR) {
		currentState = EquipmentState::READY;
		logger.Log("Reset");
		return true;
	}
	else {
		std::cout << "[ERROR] Reset command rejected.\n";
		return false;
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

void EquipmentController::PrintEventLogs() {
	logger.PrintEventLogs();
}

void EquipmentController::PrintAlarmHistory() {
	alarmManager.PrintAlarmHistory();
}