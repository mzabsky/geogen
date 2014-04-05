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
	BooleanTypeDefinition const* booleanTypeDefinition = dynamic_cast<BooleanTypeDefinition const*>(vm->GetCompiledScript().GetTypeDefinitions().GetItem("Boolean"));
	if (booleanTypeDefinition == NULL)
	{
		throw InternalErrorException("Could not get \"Boolean\" type definition.");
	}

	DynamicObject* object = booleanTypeDefinition->CreateInstance(this->constBoolean);
	vm->GetMemoryManager().RegisterObject(object);
	vm->GetObjectStack().push(object);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}