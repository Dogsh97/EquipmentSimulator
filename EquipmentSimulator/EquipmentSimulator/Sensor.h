#pragma once

class Sensor {
	private:
		bool waferDetected;
	
	public:
		Sensor();

		void DetectWafer();
		void RemoveWafer();
		bool IsDetected() const;
		void PrintStatus() const;
};