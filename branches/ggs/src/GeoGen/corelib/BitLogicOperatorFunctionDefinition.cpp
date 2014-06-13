#include "BitLogicOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "..\runtime\IncorrectTypeException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

BitLogicOperatorFunctionDefinition* BitLogicOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case BIT_AND: return new BitLogicOperatorFunctionDefinition(GG_STR("&"), CallOperatorBitAnd);
	case BIT_OR: return new BitLogicOperatorFunctionDefinition(GG_STR("|"), CallOperatorBitOr);
	case BIT_XOR: return new BitLogicOperatorFunctionDefinition(GG_STR("^"), CallOperatorBitXor);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* BitLogicOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	CheckArguments(location, 2, arguments);

	bool returnsNumber = false;
	std::vector<int> values;
	for (unsigned i = 0; i < arguments.size(); i++)
	{
		ManagedObject* arg = arguments[i];

		if (arg->IsStaticObject())
		{
			throw IncorrectTypeException(GGE2102_IncorrectOperandType, location, numberTypeDefinition->GetName(), GG_STR("Static"));
		}

		if (arg->GetType() == numberTypeDefinition)
		{
			returnsNumber = true;
			values.push_back((int)dynamic_cast<NumberObject*>(arg)->GetValue());
		}
		else if (arg->GetType() == booleanTypeDefinition){
			values.push_back(dynamic_cast<BooleanObject*>(arg)->GetValue());
		}
		else
		{
			throw IncorrectTypeException(GGE2102_IncorrectOperandType, location, numberTypeDefinition->GetName(), arg->GetType()->GetName());
		}
	}

	int result = this->function(location, values[0], values[1]);

	ManagedObject* returnObject;
	if (returnsNumber)
	{
		returnObject = numberTypeDefinition->CreateInstance(vm, result);
	}
	else
	{
		returnObject = booleanTypeDefinition->CreateInstance(vm, result > 0);
	}
	
	return returnObject;
}

int BitLogicOperatorFunctionDefinition::CallOperatorBitAnd(CodeLocation location, int a, int b)
{
	return a & b;
}

int BitLogicOperatorFunctionDefinition::CallOperatorBitOr(CodeLocation location, int a, int b)
{
	return a | b;
}

int BitLogicOperatorFunctionDefinition::CallOperatorBitXor(CodeLocation location, int a, int b)
{
	return a ^ b;
}