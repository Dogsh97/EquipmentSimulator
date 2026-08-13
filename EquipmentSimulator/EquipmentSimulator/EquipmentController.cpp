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
				if (InterlockValidation()) {
						switch (command.GetCommandType()) {
							case CommandType::Initialize:
							{
								bool isSuccess = Initialize();
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}									
								}												
								break;
							}
							case CommandType::SetRecipe:
							{
								bool isSuccess = SetRecipe(command.GetCommandRecipeId(), command.GetCommandProcessTime(), command.GetCommandTemperature());
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::CompleteInitialization:
							{
								bool isSuccess = CompleteInitialization();
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::LoadWafer:
							{
								bool isSuccess = LoadWafer(command.GetCommandWaferId());
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::Start:
							{
								bool isSuccess = Start();
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::Complete:
							{
								bool isSuccess = Complete();
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::RaiseError:
							{
								bool isSuccess = RaiseError();
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::Reset:
							{
								bool isSuccess = Reset();
								if (!isSuccess) {
									failedCommandQueue.push(command);
									EventLog eventlog(command.GetCommandType(), false);
									logger.AddEventLog(eventlog);
								}
								else {
									if (!PostValidation()) {
										failedCommandQueue.push(command);
										EventLog eventlog(command.GetCommandType(), false);
										logger.AddEventLog(eventlog);
									}
									else {
										EventLog eventlog(command.GetCommandType(), true);
										logger.AddEventLog(eventlog);
									}
								}
								break;
							}
							case CommandType::PrintState:
							{
								PrintState();
								EventLog eventlog(command.GetCommandType(), true);
								logger.AddEventLog(eventlog);
								break;
							}
						}
					}
				else{
					EventLog eventlog(command.GetCommandType(), false);
					logger.AddEventLog(eventlog);
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

bool EquipmentController::InterlockValidation() {
	switch (command.GetCommandType()) {
		case CommandType::Start:
			if (wafer.GetWaferState() != WaferState::LOADED) {
				return false;
			}
			if(sensor.IsDetected() == false){
				return false;
			}
			if (alarmManager.HasAlarm() == true) {
				return false;
			}
			return true;

		case CommandType::Complete:
			if (wafer.GetWaferState() != WaferState::PROCESSING) {
				return false;
			}
			if (sensor.IsDetected() == false) {
				return false;
			}
			if (alarmManager.HasAlarm() == true) {
				return false;
			}
			return true;
		case CommandType::RaiseError:
			if (wafer.GetWaferState() != WaferState::PROCESSING) {
				return false;
			}
			if (sensor.IsDetected() == false) {
				return false;
			}
			if (alarmManager.HasAlarm() == true) {
				return false;
			}
			return true;
		case CommandType::Reset:
			if (wafer.GetWaferState() != WaferState::PROCESSING) {
				return false;
			}
			if (sensor.IsDetected() == false) {
				return false;
			}
			if (alarmManager.HasAlarm() == false) {
				return false;
			}
			return true;
	}
	return true;
	//변수가 없는 함수들은 모두 통과되도록 설계(확장된다면 case를 늘리는 방향으로 확장성 설계)
}

bool EquipmentController::PostValidation() {
	switch (command.GetCommandType()) {
		case CommandType::Initialize:
			if (currentState == EquipmentState::INITIALIZING) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::CompleteInitialization:
			if (currentState == EquipmentState::READY) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::SetRecipe:
			if (recipe.IsSetting() == true) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::LoadWafer:
			if (currentState == EquipmentState::Loading && wafer.GetWaferState() == WaferState::LOADED && sensor.IsDetected() == true){
				return true;
			}
			else {
				return false;
			}
		case CommandType::Start:
			if (currentState == EquipmentState::RUNNING && wafer.GetWaferState() == WaferState::PROCESSING) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::Complete:
			if (currentState == EquipmentState::READY && wafer.GetWaferState() == WaferState::EMPTY && sensor.IsDetected() == false) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::Reset:
			if (currentState == EquipmentState::READY &&  alarmManager.HasAlarm() == false) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::RaiseError:
			if (currentState == EquipmentState::ERROR &&  alarmManager.HasAlarm() == true) {
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
		wafer.ResetProcess();
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
		alarmManager.ClearAlarm();
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