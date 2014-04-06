#include "ContinueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult ContinueInstruction::Step(VirtualMachine* vm) const
{
	CodeBlockStack& codeBlockStack = vm->GetCallStack().Top().GetCodeBlockStack();

	for (unsigned i = 0; i < this->codeBlockCount; i++)
	{
		codeBlockStack.Pop();
	}

	return INSTRUCTION_STEP_RESULT_TYPE_CONTINUE;
}