#include "RelationalOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

RelationalOperatorFunctionDefinition* RelationalOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case LESS_THAN: return new RelationalOperatorFunctionDefinition("<", CallOperatorLessThan);
	case LESS_THAN_OR_EQUAL_TO: return new RelationalOperatorFunctionDefinition("<=", CallOperatorLessThanOrEqualTo);
	case GREATER_THAN: return new RelationalOperatorFunctionDefinition(">", CallOperatorGreaterThan);
	case GREATER_THAN_OR_EQUAL_TO: return new RelationalOperatorFunctionDefinition(">=", CallOperatorGreaterThanOrEqualTo);
	default: throw InternalErrorException("Unknown operator type.");
	}
}

DynamicObject* RelationalOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<DynamicObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);
	NumberObject* b = dynamic_cast<NumberObject*>(arguments[1]);

	bool result = this->function(location, a->GetValue(), b->GetValue());

	DynamicObject* returnObject = booleanTypeDefinition->CreateInstance(result);
	vm->GetMemoryManager().RegisterObject(returnObject);
	return returnObject;
}

bool RelationalOperatorFunctionDefinition::CallOperatorLessThan(CodeLocation location, Number a, Number b)
{
	return a < b;
}

bool RelationalOperatorFunctionDefinition::CallOperatorLessThanOrEqualTo(CodeLocation location, Number a, Number b)
{
	return a <= b;
}

bool RelationalOperatorFunctionDefinition::CallOperatorGreaterThan(CodeLocation location, Number a, Number b)
{
	return a > b;
}

bool RelationalOperatorFunctionDefinition::CallOperatorGreaterThanOrEqualTo(CodeLocation location, Number a, Number b)
{
	return a >= b;
}