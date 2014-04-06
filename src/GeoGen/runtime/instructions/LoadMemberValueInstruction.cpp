#include "LoadMemberValueInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\NullReferenceException.hpp"
#include "..\DynamicObject.hpp"
#include "..\UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadMemberValueInstruction::Step(VirtualMachine* vm) const
{
	DynamicObject* instance = vm->GetObjectStack().top();
	vm->GetObjectStack().pop();

	if (instance == vm->GetNull())
	{
		throw NullReferenceException(this->GetLocation());
	}

	DynamicObject* memberValue = instance->GetMemberValue(*vm, this->GetLocation(), this->variableName); 
	if (memberValue == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2203_UndefinedMemberVariable, this->GetLocation(), this->variableName);
	}

	vm->GetObjectStack().push(memberValue);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}