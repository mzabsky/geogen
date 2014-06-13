#include "LoadConstStringInstruction.hpp"
#include "../VirtualMachine.hpp"
#include "../../InternalErrorException.hpp"
#include "../../corelib\StringTypeDefinition.hpp"

using namespace std;
using namespace geogen::runtime;
using namespace geogen::corelib;
using namespace geogen::runtime::instructions;

InstructionStepResult LoadConstStringInstruction::Step(VirtualMachine* vm) const
{
	StringTypeDefinition const* stringTypeDefinition = dynamic_cast<StringTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("String")));

	ManagedObject* object = stringTypeDefinition->CreateInstance(vm, this->constString);
	vm->GetObjectStack().Push(this->GetLocation(), object);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}