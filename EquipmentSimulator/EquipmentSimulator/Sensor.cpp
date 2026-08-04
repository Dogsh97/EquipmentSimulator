#include "Sensor.h"
#include <iostream>

Sensor::Sensor()
	:waferDetected(false)
{

}

void Sensor::DetectWafer() {
		waferDetected = true;
}

void Sensor::RemoveWafer() {
		waferDetected = false;
}

bool Sensor::IsDetected() const{
		return waferDetected;
}

void Sensor::PrintStatus() const {
	if (waferDetected) {
		std::cout << "Sensor : Detected \n";
	}
	else {
		std::cout << "Sensor : Not Detected \n";
	}
}