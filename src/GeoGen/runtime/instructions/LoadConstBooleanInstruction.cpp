#include "LoadConstBooleanInstruction.hpp"
#include "../VirtualMachine.hpp"
#include "../../InternalErrorException.hpp"
#include "../../corelib/BooleanTypeDefinition.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::corelib;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadConstBooleanInstruction::Step(VirtualMachine* vm) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	ManagedObject* object = booleanTypeDefinition->CreateInstance(vm, this->constBoolean);
	vm->GetObjectStack().Push(this->GetLocation(), object);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}