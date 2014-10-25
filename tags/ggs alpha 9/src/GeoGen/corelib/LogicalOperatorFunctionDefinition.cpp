#include "LogicalOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

LogicalOperatorFunctionDefinition* LogicalOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case LOGICAL_AND: return new LogicalOperatorFunctionDefinition(GG_STR("&&"), CallOperatorLogicalAnd);
	case LOGICAL_OR: return new LogicalOperatorFunctionDefinition(GG_STR("||"), CallOperatorLogicalOr);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* LogicalOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(booleanTypeDefinition);
	expectedTypes.push_back(booleanTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	BooleanObject* a = dynamic_cast<BooleanObject*>(arguments[0]);
	BooleanObject* b = dynamic_cast<BooleanObject*>(arguments[1]);

	bool result = this->function(location, a->GetValue(), b->GetValue());

	ManagedObject* returnObject = booleanTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

bool LogicalOperatorFunctionDefinition::CallOperatorLogicalAnd(CodeLocation location, bool a, bool b)
{
	return a && b;
}

bool LogicalOperatorFunctionDefinition::CallOperatorLogicalOr(CodeLocation location, bool a, bool b)
{
	return a || b;
}