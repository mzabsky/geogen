#include "MathLogPowFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "..\InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

MathLogPowFunctionDefinition* MathLogPowFunctionDefinition::Create(Function function)
{
	switch (function)
	{
	case LOG:
		return new MathLogPowFunctionDefinition(GG_STR("Log"), function);
	case POW:
		return new MathLogPowFunctionDefinition(GG_STR("Pow"), function);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* MathLogPowFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(numberTypeDefinition);
	expectedParameterTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedParameterTypes, arguments);

	Number arg1 = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	Number arg2 = dynamic_cast<NumberObject*>(arguments[1])->GetValue();

	Number result;
	switch (this->function)
	{
	case LOG:
		if (arg1 <= 0 || arg2 <= 0) throw MathDefinitionRangeException(GGE2306_OutsideLogFunctionDefinitionRange, location, this->GetName(), arg1);
		result = log(arg1) / log(arg2);
		break;
	case POW:
		result = pow(arg1, arg2);
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}