#include "CallMemberInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult CallMemberInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().Push(vm->GetNull());

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}