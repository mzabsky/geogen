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

	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	VariableTableItem* variableTableItem = instance->GetMemberVariableTable().GetVariable(this->variableName);
	if (variableTableItem == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, this->GetLocation(), this->variableName);
	}	

	ManagedObject* memberObject = variableTableItem->GetValue();
	memberObject->AddRef();

	vm->GetObjectStack().Pop(vm);

	vm->GetObjectStack().Push(this->GetLocation(), memberObject);

	memberObject->RemoveRef(vm->GetMemoryManager());
	
	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}