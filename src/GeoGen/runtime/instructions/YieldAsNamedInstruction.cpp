#include "YieldAsNamedInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult YieldAsNamedInstruction::Step(VirtualMachine* vm) const
{
	DynamicObject* yieldedValue = vm->GetObjectStack().Top();
	vm->GetObjectStack().Pop();

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}