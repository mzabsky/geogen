#include "UnaryArithmeticOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../runtime/DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

UnaryArithmeticOperatorFunctionDefinition* UnaryArithmeticOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case UNARY_PLUS: return new UnaryArithmeticOperatorFunctionDefinition("+un", CallOperatorUnaryPlus);
	case UNARY_MINUS: return new UnaryArithmeticOperatorFunctionDefinition("-un", CallOperatorUnaryMinus);
	default: throw InternalErrorException("Unknown operator type.");
	}
}

DynamicObject* UnaryArithmeticOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<DynamicObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);

	Number result = this->function(location, a->GetValue());

	DynamicObject* returnObject = numberTypeDefinition->CreateInstance(result);
	vm->GetMemoryManager().RegisterObject(returnObject);
	return returnObject;
}

Number UnaryArithmeticOperatorFunctionDefinition::CallOperatorUnaryPlus(CodeLocation location, Number a)
{
	return a;
}

Number UnaryArithmeticOperatorFunctionDefinition::CallOperatorUnaryMinus(CodeLocation location, Number a)
{
	return -a;
}