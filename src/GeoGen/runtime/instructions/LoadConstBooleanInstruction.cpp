#include "LoadConstBooleanInstruction.hpp"
#include "..\CodeBlockStackEntry.hpp"
#include "..\VirtualMachine.hpp"
#include "..\..\InternalErrorException.hpp"
#include "..\BooleanTypeDefinition.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadConstBooleanInstruction::Step(VirtualMachine* vm) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	DynamicObject* object = booleanTypeDefinition->CreateInstance(vm, this->constBoolean);
	vm->GetMemoryManager().RegisterObject(object);
	vm->GetObjectStack().Push(this->GetLocation(), object);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}