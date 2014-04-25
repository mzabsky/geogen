#include "LoadMemberValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\NullReferenceException.hpp"
#include "..\ManagedObject.hpp"
#include "..\UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadMemberValueInstruction::Step(VirtualMachine* vm) const
{
	ManagedObject* instance = vm->GetObjectStack().Top();
	vm->GetObjectStack().Pop();

	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	VariableTableItem* variableTableItem = instance->GetMemberVariableTable().GetVariable(this->variableName);
	if (variableTableItem == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, this->GetLocation(), this->variableName);
	}

	vm->GetObjectStack().Push(this->GetLocation(), variableTableItem->GetValue());

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}