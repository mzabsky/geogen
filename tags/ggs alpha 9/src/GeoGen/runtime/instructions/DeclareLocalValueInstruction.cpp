#include "DeclareLocalValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\VariableRedefinitionException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult DeclareLocalValueInstruction::Step(VirtualMachine* vm) const
{
	if (!vm->GetCallStack().Top().GetCodeBlockStack().Top().GetLocalVariableTable().DeclareVariable(this->variableName, vm->GetNull(), false))
	{
		throw VariableRedefinitionException(this->GetLocation(), this->variableName);
	}

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}