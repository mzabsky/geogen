#include "VirtualMachine.hpp"

using namespace geogen::runtime;

VirtualMachineStepResult VirtualMachine::Step()
{
	return VIRTUAL_MACHINE_STEP_RESULT_RUNNING;
}