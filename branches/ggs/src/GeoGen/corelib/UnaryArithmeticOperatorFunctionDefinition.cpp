#include "UnaryArithmeticOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

UnaryArithmeticOperatorFunctionDefinition* UnaryArithmeticOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case UNARY_PLUS: return new UnaryArithmeticOperatorFunctionDefinition(GG_STR("+un"), CallOperatorUnaryPlus);
	case UNARY_MINUS: return new UnaryArithmeticOperatorFunctionDefinition(GG_STR("-un"), CallOperatorUnaryMinus);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* UnaryArithmeticOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);

	Number result = this->function(location, a->GetValue());

	ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, result);
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