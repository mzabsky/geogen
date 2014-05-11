#include "EqualityOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

EqualityOperatorFunctionDefinition* EqualityOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case EQUAL_TO: return new EqualityOperatorFunctionDefinition("==", CallOperatorEqualTo);
	case NOT_EQUAL_TO: return new EqualityOperatorFunctionDefinition("!=", CallOperatorNotEqualTo);
	default: throw InternalErrorException("Unknown operator type.");
	}
}

ManagedObject* EqualityOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	this->CheckArguments(location, 2, arguments);

	bool result = this->function(location, arguments[0], arguments[1]);

	ManagedObject* returnObject = booleanTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

bool EqualityOperatorFunctionDefinition::CallOperatorEqualTo(CodeLocation location, ManagedObject* a, ManagedObject* b)
{
	return a->GetType()->InstanceEqualsTo(a, b);
}

bool EqualityOperatorFunctionDefinition::CallOperatorNotEqualTo(CodeLocation location, ManagedObject* a, ManagedObject* b)
{
	return !a->GetType()->InstanceEqualsTo(a, b);
}