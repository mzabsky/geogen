#include "MathRoundFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "..\InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

MathRoundFunctionDefinition* MathRoundFunctionDefinition::Create(Function function)
{
	switch (function)
	{
	case ROUND:
		return new MathRoundFunctionDefinition("Round", function);
	case CEIL:
		return new MathRoundFunctionDefinition("Ceil", function);
	case FLOOR:
		return new MathRoundFunctionDefinition("Floor", function);
	case ABS:
		return new MathRoundFunctionDefinition("Abs", function);
	default: throw InternalErrorException("Unknown method.");
	}
}

ManagedObject* MathRoundFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedParameterTypes, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	Number result;
	switch (this->function)
	{
	case ROUND:
		result = round(input);
		break;
	case FLOOR:
		result = floor(input);
		break;
	case CEIL:
		result = ceil(input);
		break;
	case ABS:
		result = abs(input);
		break;
	default:
		throw InternalErrorException("Unknown method.");
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}