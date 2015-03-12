#include <cmath>
#include <limits>

#include "BinaryArithmeticOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "DivisionByZeroException.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/NumberOverflowException.hpp"
#include "../runtime/NumberUnderflowException.hpp"

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

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);
	NumberObject* b = dynamic_cast<NumberObject*>(arguments[1]);

	//RuntimeMathCheckInit();

	Number result = this->function(location, a->GetValue(), b->GetValue());

	//RuntimeMathCheck(location);

    ManagedObject* returnObject = numberTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorMultiplication(CodeLocation location, Number a, Number b)
{
	Number result = a * b;
	if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
	{
		throw NumberOverflowException(location);
	}
	else if (a != 0 && b != 0 && result == 0)
	{
		throw NumberUnderflowException(location);
	}

	return result;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorDivision(CodeLocation location, Number a, Number b)
{
	if (b == 0)
	{
		throw DivisionByZeroException(location);
	}

	Number result = a / b;

	if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
	{
		throw NumberOverflowException(location);
	}
	else if (a != 0 && result == 0)
	{
		throw NumberUnderflowException(location);
	}

	return result;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorModulo(CodeLocation location, Number a, Number b)
{
	if (b == 0)
	{
		throw DivisionByZeroException(location);
	}

	if (a > numeric_limits<int>::max() || a < numeric_limits<int>::min() || b > numeric_limits<int>::max() || b < numeric_limits<int>::min())
	{
		throw NumberOverflowException(location);
	}

	return (int)a % (int)b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorAddition(CodeLocation location, Number a, Number b)
{
	Number result = a + b;

	if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
	{
		throw NumberOverflowException(location);
	}

	return result;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorSubtraction(CodeLocation location, Number a, Number b)
{
	Number result = a - b;

	if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
	{
		throw NumberOverflowException(location);
	}

	return result;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorBitShiftLeft(CodeLocation location, Number a, Number b)
{
	if (a > numeric_limits<int>::max() || a < numeric_limits<int>::min() || b > numeric_limits<int>::max() || b < numeric_limits<int>::min())
	{
		throw NumberOverflowException(location);
	}

	return (int)a << (int)b;
}

Number BinaryArithmeticOperatorFunctionDefinition::CallOperatorBitShiftRight(CodeLocation location, Number a, Number b)
{
	if (a > numeric_limits<int>::max() || a < numeric_limits<int>::min() || b > numeric_limits<int>::max() || b < numeric_limits<int>::min())
	{
		throw NumberOverflowException(location);
	}

	return (int)a >> (int)b;
}