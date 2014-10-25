#include "LoadNullInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadNullInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().Push(this->GetLocation(), vm->GetNull());

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}