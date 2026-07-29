#include <iostream>
#include "EquipmentController.h"

int main()
{
	EquipmentController equipment;

	
	equipment.PrintState();
	equipment.Initialize();
	equipment.PrintState();
	equipment.CompleteInitialization();
	equipment.PrintState();
	equipment.LoadWafer(1001);
	equipment.PrintState();
	equipment.Start();
	equipment.PrintState();
	equipment.Complete();
	equipment.PrintState();
	equipment.Start();
	equipment.PrintState();
	equipment.RaiseError();
	equipment.PrintState();
	equipment.Reset();
	equipment.PrintState();

	return 0;
}
