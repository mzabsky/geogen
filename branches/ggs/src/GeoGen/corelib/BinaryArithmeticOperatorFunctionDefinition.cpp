#include "BinaryArithmeticOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../runtime/DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

BinaryArithmeticOperatorFunctionDefinition* BinaryArithmeticOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case MULTIPLICATION: return new BinaryArithmeticOperatorFunctionDefinition("*", CallOperatorMultiplication);
	case DIVISION: return new BinaryArithmeticOperatorFunctionDefinition("/", CallOperatorDivision);
	case MODULO: return new BinaryArithmeticOperatorFunctionDefinition("%", CallOperatorModulo);
	case ADDITION: return new BinaryArithmeticOperatorFunctionDefinition("+", CallOperatorAddition);
	case SUBTRACTION: return new BinaryArithmeticOperatorFunctionDefinition("-", CallOperatorSubtraction);
	case BIT_SHIFT_LEFT: return new BinaryArithmeticOperatorFunctionDefinition("<<", CallOperatorBitShiftLeft);
	case BIT_SHIFT_RIGHT: return new BinaryArithmeticOperatorFunctionDefinition(">>", CallOperatorBitShiftRight);
	default: throw InternalErrorException("Unknown operator type.");
	}
}

DynamicObject* BinaryArithmeticOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<DynamicObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
    
    vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);
	NumberObject* b = dynamic_cast<NumberObject*>(arguments[1]);

	Number result = this->function(location, a->GetValue(), b->GetValue());
	
    DynamicObject* returnObject = numberTypeDefinition->CreateInstance(result);
	vm->GetMemoryManager().RegisterObject(returnObject);
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