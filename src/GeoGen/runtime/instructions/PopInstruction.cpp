#include "PopInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult PopInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().Pop(vm);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}