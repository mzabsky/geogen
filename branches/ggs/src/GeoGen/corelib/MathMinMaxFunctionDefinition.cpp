#include "MathMinMaxFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "..\InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

MathMinMaxFunctionDefinition* MathMinMaxFunctionDefinition::Create(Function function)
{
	switch (function)
	{
	case MIN:
		return new MathMinMaxFunctionDefinition(GG_STR("Min"), function);
	case MAX:
		return new MathMinMaxFunctionDefinition(GG_STR("Max"), function);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

Number MathMinMaxFunctionDefinition::Min(std::vector<runtime::ManagedObject*> arguments) const
{
	Number result = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	for (unsigned i = 1; i < arguments.size(); i++)
	{
		if (result > dynamic_cast<NumberObject*>(arguments[i])->GetValue())
		{
			result = dynamic_cast<NumberObject*>(arguments[i])->GetValue();
		}
	}

	return result;
}

Number MathMinMaxFunctionDefinition::Max(std::vector<runtime::ManagedObject*> arguments) const
{
	Number result = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	for (unsigned i = 1; i < arguments.size(); i++)
	{
		if (result < dynamic_cast<NumberObject*>(arguments[i])->GetValue())
		{
			result = dynamic_cast<NumberObject*>(arguments[i])->GetValue();
		}
	}

	return result;
}

ManagedObject* MathMinMaxFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	int parameterCount = max(2u, arguments.size());
	vector<TypeDefinition const*> expectedParameterTypes;

	for (int i = 0; i < parameterCount; i++)
	{
		expectedParameterTypes.push_back(numberTypeDefinition);
	}

	this->CheckArguments(location, expectedParameterTypes, arguments);

	Number result;
	switch (this->function)
	{
	case MIN:
		result = this->Min(arguments);
		break;
	case MAX:
		result = this->Max(arguments);
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}