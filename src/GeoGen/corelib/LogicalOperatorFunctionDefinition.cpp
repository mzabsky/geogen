#include "LogicalOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

LogicalOperatorFunctionDefinition* LogicalOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case LOGICAL_AND: return new LogicalOperatorFunctionDefinition("&&", CallOperatorLogicalAnd);
	case LOGICAL_OR: return new LogicalOperatorFunctionDefinition("||", CallOperatorLogicalOr);
	default: throw InternalErrorException("Unknown operator type.");
	}
}

DynamicObject* LogicalOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<DynamicObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(booleanTypeDefinition);
	expectedTypes.push_back(booleanTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	BooleanObject* a = dynamic_cast<BooleanObject*>(arguments[0]);
	BooleanObject* b = dynamic_cast<BooleanObject*>(arguments[1]);

	bool result = this->function(location, a->GetValue(), b->GetValue());

	DynamicObject* returnObject = booleanTypeDefinition->CreateInstance(result);
	vm->GetMemoryManager().RegisterObject(returnObject);
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