#include "StoreMemberValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\NullReferenceException.hpp"
#include "..\DynamicObject.hpp"
#include "..\UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult StoreMemberValueInstruction::Step(VirtualMachine* vm) const
{
	DynamicObject* instance = vm->GetObjectStack().Top();
	vm->GetObjectStack().Pop();

	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	DynamicObject* value = vm->GetObjectStack().Top();
	vm->GetObjectStack().Pop();

	if (instance->SetMemberValue(*vm, this->GetLocation(), this->variableName, value))
	{
		throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, this->GetLocation(), this->variableName);
	}

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}