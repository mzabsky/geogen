#include "CallMemberInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\UndefinedSymbolAccessException.hpp"
#include "..\ManagedObject.hpp"
#include "..\NullReferenceException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult CallMemberInstruction::Step(VirtualMachine* vm) const
{
	vm->GetObjectStack().CheckSize(1);
	ManagedObject* instance = vm->GetObjectStack().Top();
	instance->AddRef();
	vm->GetObjectStack().Pop(vm);

	FunctionDefinition const* functionDefinition;
	
	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	if (instance->IsStaticObject())
	{
		functionDefinition = instance->GetType()->GetStaticFunctionDefinitions().GetItem(this->functionName);
	}
	else
	{
		functionDefinition = instance->GetType()->GetFunctionDefinitions().GetItem(this->functionName);
	}
	
	if (functionDefinition == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2201_UndefinedFunction, this->GetLocation(), this->functionName);
	}

	vm->CallFunction(this->GetLocation(), functionDefinition, instance, this->argumentCount);

	instance->RemoveRef(vm->GetMemoryManager());

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}