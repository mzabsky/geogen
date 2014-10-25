#include "LoadScopeReferenceInstruction.hpp"
#include "../VirtualMachine.hpp"
#include "../../InternalErrorException.hpp"
#include "../../corelib/ReferenceTypeDefinition.hpp"
#include "../UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;
using namespace instructions;

InstructionStepResult LoadScopeReferenceInstruction::Step(VirtualMachine* vm) const
{
	ReferenceTypeDefinition const* referenceTypeDefinition = dynamic_cast<ReferenceTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("<Reference>")));

	VariableTableItem* variable = vm->FindVariable(this->variableName);
	if (variable == NULL)
	{
		throw UndefinedSymbolAccessException(GGE2202_UndefinedVariable, this->GetLocation(), this->variableName);
	}

	ManagedObject* object = referenceTypeDefinition->CreateScopeReferenceInstance(vm, variable);
	vm->GetObjectStack().Push(this->GetLocation(), object);

	return INSTRUCTION_STEP_RESULT_TYPE_NORMAL;
}