#pragma once

enum class WaferState {
	EMPTY,
	LOADED,
	PROCESSING,
	COMPLETED
};

class Wafer {
	private:
		int waferId;
		WaferState state;

	public:
		Wafer();

		void Load(int id);
		void StartProcessing();
		void CompleteProcess();
		void PrintInfo();
		WaferState GetWaferState();
};