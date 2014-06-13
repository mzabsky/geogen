#include "BinaryArithmeticOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

BinaryArithmeticOperatorFunctionDefinition* BinaryArithmeticOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case MULTIPLICATION: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR("*"), CallOperatorMultiplication);
	case DIVISION: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR("/"), CallOperatorDivision);
	case MODULO: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR("%"), CallOperatorModulo);
	case ADDITION: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR("+"), CallOperatorAddition);
	case SUBTRACTION: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR("-"), CallOperatorSubtraction);
	case BIT_SHIFT_LEFT: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR("<<"), CallOperatorBitShiftLeft);
	case BIT_SHIFT_RIGHT: return new BinaryArithmeticOperatorFunctionDefinition(GG_STR(">>"), CallOperatorBitShiftRight);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* BinaryArithmeticOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
    
    vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);
	NumberObject* b = dynamic_cast<NumberObject*>(arguments[1]);

	Number result = this->function(location, a->GetValue(), b->GetValue());
	
    ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorMultiplication(CodeLocation location, Number a, Number b)
{
	return a * b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorDivision(CodeLocation location, Number a, Number b)
{
	if (b == 0)
	{
		throw DivisionByZeroException(location);
	}

	return a / b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorModulo(CodeLocation location, Number a, Number b)
{
	if (b == 0)
	{
		throw DivisionByZeroException(location);
	}

	return (int)a % (int)b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorAddition(CodeLocation location, Number a, Number b)
{
	return a + b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorSubtraction(CodeLocation location, Number a, Number b)
{
	return a - b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorBitShiftLeft(CodeLocation location, Number a, Number b)
{
	return (int)a << (int)b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorBitShiftRight(CodeLocation location, Number a, Number b)
{
	return (int)a >> (int)b;
}