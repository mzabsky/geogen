#include "CallGlobalInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult CallGlobalInstruction::Step(VirtualMachine* vm) const
{
	FunctionDefinition const* functionDefinition = vm->GetCompiledScript().GetGlobalFunctionDefinitions().GetItem(this->functionName);
	if (functionDefinition == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2201_UndefinedFunction, this->GetLocation(), this->functionName);
	}

	vm->CallFunction(this->GetLocation(), functionDefinition, this->argumentCount);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}