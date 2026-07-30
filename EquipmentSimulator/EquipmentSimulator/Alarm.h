#pragma once
enum class AlarmCode {
	NONE,
	WAFER_NOT_DETECTED,
	EQUIPMENT_NOT_READY,
	PROCESS_ALREADY_RUNNING
};


class AlarmManager {
	private:
		AlarmCode currentAlarm;
		bool AlarmDetected;
	public:
		AlarmManager();
		void RaiseAlarm(AlarmCode code);
		void ClearAlarm();
		bool HasAlarm() const;
		void PrintAlarm() const;
};