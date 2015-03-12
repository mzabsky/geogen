#include <cmath>

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
		return new MathRoundFunctionDefinition(GG_STR("Round"), function);
	case CEIL:
		return new MathRoundFunctionDefinition(GG_STR("Ceil"), function);
	case FLOOR:
		return new MathRoundFunctionDefinition(GG_STR("Floor"), function);
	case ABS:
		return new MathRoundFunctionDefinition(GG_STR("Abs"), function);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* MathRoundFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameterTypes, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	Number result;
	switch (this->function)
	{
	case ROUND:
		result = input > 0 ? std::floor(input + 0.5) : std::ceil(input - 0.5); // C++98 doesn't have std::round
		break;
	case FLOOR:
		result = std::floor(input);
		break;
	case CEIL:
		result = std::ceil(input);
		break;
	case ABS:
		result = std::abs(input);
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}