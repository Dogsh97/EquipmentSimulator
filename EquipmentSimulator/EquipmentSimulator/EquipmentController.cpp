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

// Command를 실제로 실행한 뒤 PostValidation을 통해 실행 결과가 기대 상태에 도달했는지 확인한다.
// PostValidation에 실패한 Command는 FailedCommandQueue에 저장하고 실패 원인을 EventLog에 기록한다.
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

// CommandType에 따라 실제 장비 동작 함수를 호출한다.
// Command의 사전 검증과 실행 후 검증은 상위 실행 흐름에서 담당한다.
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

// Command는 Parameter → EquipmentState → Interlock 순으로 사전 검증한다.
// 모든 사전 검증을 통과한 경우에만 Execute를 수행하고, Execute 이후 PostValidation을 통해 실행 결과가 기대 상태에 도달했는지 확인한다.
// 사전 검증에 실패한 Command는 EventLog에 실패 원인을 기록한 후 CommandQueue에서 제거한다.
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

// FailedCommandQueue의 Command를 Retry 대상으로 다시 등록한다.
// Retry 기능 횟수를 초과한 Command는 재등록 하지 않는다.
// Retry 대상 Command는 별도의 실행 로직을 사용하지 않고 기존 RunCommand()를 통해 동일한 Validation 및 Execute Flow를 재사용한다.
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

// 현재 EquipmentState에서 Command를 실행할 수 있는지 확인한다.
// 장비 상태가 실행 조건을 만족하지 않는 경우 Command를 실행하지 않는다.
// 장비 이상 또는 운전 조건 위반으로 판단되는 경우 RaiseAlarm()을 통해 Alarm을 발생 시키고 false를 반환한다.
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

// Command 실행에 필요한 입력 파라미터가 유효한지 확인한다.
// 현재는 SetRecipe와 LoadWafer의 입력값을 검증하며, 별도의 파라미터가 없는 Command는 검증을 통과시킨다.
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
}

// Command 실행 직전 Sensor 및 Alarm 상태를 확인하여 장비 운전 조건을 만족하는지 검증한다.
// 장비 이상 또는 운전 조건 위반으로 판단되는 경우 RaiseAlarm()을 통해 Alarm을 발생시키고 실패를 반환한다.
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
}

// Execute 이후 장비가 Command 별 기대 상태에 도달했는지 확인한다.
// EquipmentState, WaferState, Sensor, Alarm 상태를 확인하여 실제 실행 결과가 설계된 상태와 일치하는지 확인한다.
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

// ERROR 상태를 READY 상태로 복구하고 Wafer, Sensor, Alarm 상태를 초기 상태로 복원한다.
void EquipmentController::Reset() {
		currentState = EquipmentState::READY;		
		wafer.ResetProcess();
		sensor.RemoveWafer();
		alarmManager.ClearAlarm();
		logger.Log("Reset");
}

// 장비 이상 또는 운전 조건 위반 시 Alarm을 발생시키고 사용자에게 즉시 알린 후 Validation 실패를 반환한다.
bool EquipmentController::RaiseAlarm(AlarmCode code)
{
	alarmManager.RaiseAlarm(code);
	alarmManager.PrintAlarm();

	return false;
}

//현재 장비의 Wafer, Sensor, Alarm, Log 정보를 출력한다.
void EquipmentController::PrintEquipmentInfo() {
	wafer.PrintInfo();
	sensor.PrintStatus();
	alarmManager.PrintAlarm();
	logger.PrintLog();
}

std::string EquipmentStateToString(EquipmentState state)
{
	switch (state)
	{
	case EquipmentState::IDLE:
		return "IDLE";
	case EquipmentState::INITIALIZING:
		return "INITIALIZING";
	case EquipmentState::Loading:
		return "Loading";
	case EquipmentState::READY:
		return "READY";
	case EquipmentState::RUNNING:
		return "RUNNING";
	case EquipmentState::ERROR:
		return "ERROR";
	}

	return "UNKNOWN";
}

void EquipmentController::PrintState(){ 
	PrintEquipmentInfo();
	std::cout << "Equipment State : " << EquipmentStateToString(currentState) << '\n';
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
