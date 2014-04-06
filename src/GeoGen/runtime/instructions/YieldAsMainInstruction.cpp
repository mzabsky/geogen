#include "YieldAsMainInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult YieldAsMainInstruction::Step(VirtualMachine* vm) const
{
	DynamicObject* yieldedValue = vm->GetObjectStack().top();
	vm->GetObjectStack().pop();

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}