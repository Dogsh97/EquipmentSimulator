#include <iostream>
#include "EquipmentController.h"
#include <string>

EquipmentController::EquipmentController()
	: currentState(EquipmentState::IDLE)
{
}

void EquipmentController::MakeCommand(Command command) {
	commandQueue.PushCommand(command);
}

void EquipmentController::ProcessCommandResult(Command command) {
	ExecuteCommand(command);
	
	if (!PostValidation(command)) {
		failedCommandQueue.push(command);
		AddEventLog(command, false, CommandResultType::PostValidationFailed);
		return;
	}

	AddEventLog(command, true, CommandResultType::Success);
}

void EquipmentController::AddEventLog(Command command, bool success, CommandResultType type) {
	EventLog eventlog(command.GetCommandType(), success, type);
	logger.AddEventLog(eventlog);
}

void EquipmentController::ExecuteCommand(Command command){
	CommandType commandType = command.GetCommandType();

	switch (commandType) {
		case CommandType::Initialize:
		{
			 Initialize();
			 break;
		}
		case CommandType::SetRecipe:
		{
			SetRecipe(command.GetCommandRecipeId(), command.GetCommandProcessTime(), command.GetCommandTemperature());
			break;
		}
		case CommandType::CompleteInitialization:
		{
			CompleteInitialization();
			break;
		}
		case CommandType::LoadWafer:
		{
			LoadWafer(command.GetCommandWaferId());
			break;
		}
		case CommandType::Start:
		{
			Start();
			break;
		}
		case CommandType::Complete:
		{
			Complete();
			break;
		}
		case CommandType::RaiseError:
		{
			RaiseError();
			break;
		}
		case CommandType::Reset:
		{
			Reset();
			break;
		}
		case CommandType::PrintState:
		{
			PrintState();
			break;
		}
	}	
}
void EquipmentController::RunCommand() {
	while(commandQueue.CommandDetected()) {
		Command currentCommand = commandQueue.GetCommand();

		if (!CommandParameterValidation(currentCommand)) {
			AddEventLog(currentCommand, false, CommandResultType::ParameterValidationFailed);
			commandQueue.PopCommand();
			continue;
		}

		if (!CanExecute(currentCommand, currentState)) {
			AddEventLog(currentCommand, false, CommandResultType::CanExecuteFailed);
			commandQueue.PopCommand();
			continue;
		}

		if (!InterlockValidation(currentCommand)) {
			AddEventLog(currentCommand, false, CommandResultType::InterlockFailed);
			commandQueue.PopCommand();
			continue;
		}

		ProcessCommandResult(currentCommand);

		commandQueue.PopCommand();		
	}
}

void EquipmentController::PrintFailedCommands() {
	std::queue<Command> temp;
	temp = failedCommandQueue;
	int index = 1;

	std::cout << "===== Failed Command Queue =====\n";

	if (temp.empty())
	{
		std::cout << "Empty\n";
		return;
	}

	while (!temp.empty()) {
		std::cout << "[" << index << "]\n";
		std::cout << "Command : ";
		temp.front().PrintCommand();
		std::cout << "\n";
		std::cout << "RetryCount : " << temp.front().GetRetryCount() << "\n";
		index++;
		temp.pop();
	}
}

void EquipmentController::RetryFailedCommands() {
		while (!failedCommandQueue.empty()) {
			Command temp(failedCommandQueue.front());
			if (temp.GetRetryCount() < RetryCountMax) {
				temp.IncreaseRetryCount();
				//std::cout << "RetryCount : " << temp.GetRetryCount() << "\n";
				commandQueue.PushCommand(temp);
			}
			failedCommandQueue.pop();
		}
		RunCommand();
}

bool EquipmentController::CanExecute(Command command, EquipmentState state) {
	CommandType commandType = command.GetCommandType();

	switch (commandType) {
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

				return RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
			}
		case CommandType::Start:
			if (state == EquipmentState::RUNNING) {
				return RaiseAlarm(AlarmCode::PROCESS_ALREADY_RUNNING);
			}

			if (state != EquipmentState::Loading) {
				return RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
			}

			if (wafer.GetWaferState() != WaferState::LOADED)
			{
				return RaiseAlarm(AlarmCode::WAFER_NOT_DETECTED);
			}
			
			if (!recipe.IsSetting())
			{
				return RaiseAlarm(AlarmCode::RECIPE_NOT_SET);
			}
			return true;

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

		default:
			return false;
	}
	
}

bool EquipmentController::CommandParameterValidation(Command command) {
	CommandType commandType = command.GetCommandType();

	switch (commandType) {
	case CommandType::SetRecipe:
		if (command.GetCommandRecipeId() <= 0) {
			return false;
		}
		if (!recipe.IsValid(command.GetCommandProcessTime(), command.GetCommandTemperature())) {
			logger.Log("Recipe is not Valid");
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

bool EquipmentController::InterlockValidation(Command command) {
	CommandType commandType = command.GetCommandType();

	switch (commandType) {
		case CommandType::Start:
			if(!sensor.IsDetected()){
				return RaiseAlarm(AlarmCode::WAFER_NOT_DETECTED);
			}
			if (alarmManager.HasAlarm()) {
				return false;
			}
			return true;

		case CommandType::Complete:
			if (wafer.GetWaferState() != WaferState::PROCESSING) {
				return RaiseAlarm(AlarmCode::EQUIPMENT_NOT_READY);
			}
			if (!sensor.IsDetected()) {
				return RaiseAlarm(AlarmCode::WAFER_NOT_DETECTED);
			}
			if (alarmManager.HasAlarm()) {
				return false;
			}
			return true;

		case CommandType::LoadWafer:
			if (wafer.GetWaferState() != WaferState::EMPTY)
			{
				return RaiseAlarm(AlarmCode::WAFER_ALREADY);
			}

			if (sensor.IsDetected())
			{
				return RaiseAlarm(AlarmCode::WAFER_ALREADY);
			}

			if (alarmManager.HasAlarm())
			{
				return false;
			}

			return true;

		case CommandType::RaiseError:
			if (wafer.GetWaferState() != WaferState::PROCESSING) {
				return false;
			}
			if (!sensor.IsDetected()) {
				return false;
			}
			if (alarmManager.HasAlarm()) {
				return false;
			}
			return true;

		case CommandType::Reset:
			if (wafer.GetWaferState() != WaferState::PROCESSING) {
				return false;
			}
			if (!sensor.IsDetected()) {
				return false;
			}
			if (!alarmManager.HasAlarm()) {
				return false;
			}
			return true;
	}
	return true;
	//변수가 없는 함수들은 모두 통과되도록 설계(확장된다면 case를 늘리는 방향으로 확장성 설계)
}

bool EquipmentController::PostValidation(Command command) {
	CommandType commandType = command.GetCommandType();
	switch (commandType) {
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
			if (recipe.IsSetting()) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::LoadWafer:
			if (currentState == EquipmentState::Loading && wafer.GetWaferState() == WaferState::LOADED && sensor.IsDetected()){
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
			if (currentState == EquipmentState::READY && wafer.GetWaferState() == WaferState::EMPTY && !sensor.IsDetected()) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::Reset:
			if (currentState == EquipmentState::READY && wafer.GetWaferState() == WaferState::EMPTY && !sensor.IsDetected() && !alarmManager.HasAlarm()) {
				return true;
			}
			else {
				return false;
			}
		case CommandType::RaiseError:
			if (currentState == EquipmentState::ERROR &&  alarmManager.HasAlarm()) {
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

void EquipmentController::Initialize() {
		currentState = EquipmentState::INITIALIZING;
		logger.Log("INITIALIZE");
}

void EquipmentController::SetRecipe(int id, float time, float temperature) {
		recipe.SetRecipe(id, time, temperature);
		logger.Log("SetRecipe");
}

void EquipmentController::CompleteInitialization() {
		currentState = EquipmentState::READY;
		logger.Log("CompleteInitialization");
}

void EquipmentController::LoadWafer(int id) {
		wafer.Load(id);
		sensor.DetectWafer();
		currentState = EquipmentState::Loading;
		alarmManager.ClearAlarm();
		logger.Log("LoadWafer id :" + std::to_string(id));
}

void EquipmentController::Start() {
	 	currentState = EquipmentState::RUNNING;
		wafer.StartProcessing();
		alarmManager.ClearAlarm();
		logger.Log("Start");
}

void EquipmentController::Complete() {
		currentState = EquipmentState::READY;
		wafer.CompleteProcess();
		sensor.RemoveWafer();
		wafer.ResetProcess();
		logger.Log("Complete");
}

void EquipmentController::RaiseError() {
		currentState = EquipmentState::ERROR;
		alarmManager.RaiseAlarm(AlarmCode::PROCESS_ALREADY_RUNNING);
		alarmManager.PrintAlarm();
		logger.Log("RaiseError");
}

void EquipmentController::Reset() {
		currentState = EquipmentState::READY;		
		wafer.ResetProcess();
		sensor.RemoveWafer();
		alarmManager.ClearAlarm();
		logger.Log("Reset");
}

bool EquipmentController::RaiseAlarm(AlarmCode code)
{
	alarmManager.RaiseAlarm(code);
	alarmManager.PrintAlarm();

	return false;
}

void EquipmentController::PrintEquipmentInfo() {
	wafer.PrintInfo();
	sensor.PrintStatus();
	alarmManager.PrintAlarm();
	logger.PrintLog();
}

void EquipmentController::PrintState() {
	switch (currentState) {
		case EquipmentState::IDLE:
			PrintEquipmentInfo();
			std::cout << "IDLE\n";
			break;
		case EquipmentState::INITIALIZING:
			PrintEquipmentInfo();
			std::cout << "INITIALIZING\n";
			break;
		case EquipmentState::Loading:
			PrintEquipmentInfo();
			std::cout << "Loading\n";
			break;
		case EquipmentState::READY:
			PrintEquipmentInfo();
			std::cout << "READY\n";
			break;
		case EquipmentState::RUNNING :
			PrintEquipmentInfo();
			std::cout << "RUNNING\n";	
			break;
		case EquipmentState::ERROR:
			PrintEquipmentInfo();
			std::cout << "ERROR\n";
			break;
	}
}

void EquipmentController::PrintEventLogs() {
	logger.PrintEventLogs();
}

void  EquipmentController::ResetEventLogs() {
	logger.ResetEventLogs();
}

void EquipmentController::PrintAlarmHistory() {
	alarmManager.PrintAlarmHistory();
}
 
void EquipmentController::TestAddFailedCommand(Command command) {
	failedCommandQueue.push(command);
}