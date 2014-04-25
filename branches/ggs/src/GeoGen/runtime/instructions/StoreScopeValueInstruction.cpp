#include "StoreScopeValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\UndefinedSymbolAccessException.hpp"
#include "..\ReadOnlyWriteException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult StoreScopeValueInstruction::Step(VirtualMachine* vm) const
{
	VariableTableItem* variable = vm->FindVariable(this->variableName);
	if (variable == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2202_UndefinedVariable, this->GetLocation(), this->variableName);
	}

	ManagedObject* value = vm->GetObjectStack().Top();
	//vm->GetObjectStack().Pop();
	
	if (!variable->SetValue(value))
	{
		throw ReadOnlyWriteException(this->GetLocation(), this->variableName);
	}

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}