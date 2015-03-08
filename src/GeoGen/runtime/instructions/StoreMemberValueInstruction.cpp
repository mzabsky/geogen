#include "StoreMemberValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\NullReferenceException.hpp"
#include "..\ManagedObject.hpp"
#include "..\UndefinedSymbolAccessException.hpp"
#include "..\ReadOnlyWriteException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult StoreMemberValueInstruction::Step(VirtualMachine* vm) const
{
	ManagedObject* instance = vm->GetObjectStack().Top();	

	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	ManagedObject* value = vm->GetObjectStack().Top();

	VariableTableItem* variableTableItem = instance->GetMemberVariableTable().GetVariable(this->variableName);
	if (variableTableItem == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2202_UndefinedVariable, this->GetLocation(), this->variableName);
	}

	if (!variableTableItem->SetValue(vm, value))
	{
		throw ReadOnlyWriteException(this->GetLocation(), this->variableName);
	}

	vm->GetObjectStack().Pop(vm);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}