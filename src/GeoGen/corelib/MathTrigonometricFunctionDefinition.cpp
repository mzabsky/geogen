#include "MathTrigonometricFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

MathTrigonometricFunctionDefinition* MathTrigonometricFunctionDefinition::Create(Function function)
{
	switch (function)
	{
	case SIN:
		return new MathTrigonometricFunctionDefinition("Sin", function);
	case COS:
		return new MathTrigonometricFunctionDefinition("Cos", function);
	case TAN:
		return new MathTrigonometricFunctionDefinition("Tan", function);
	case ASIN:
		return new MathTrigonometricFunctionDefinition("Asin", function);
	case ACOS:
		return new MathTrigonometricFunctionDefinition("Acos", function);
	case ATAN:
		return new MathTrigonometricFunctionDefinition("Atan", function);
	case DEG_TO_RAD:
		return new MathTrigonometricFunctionDefinition("DegToRad", function);
	case RAD_TO_DEG:
		return new MathTrigonometricFunctionDefinition("RadToDeg", function);
	default: throw InternalErrorException("Unknown method.");
	}
}

ManagedObject* MathTrigonometricFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(numberTypeDefinition);

	this->CheckArguments(location, expectedParameterTypes, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	Number result;
	switch (this->function)
	{
	case SIN:
		result = sin(input);
		break;
	case COS:
		result = cos(input);
		break;
	case TAN:
		result = tan(input);
		break;
	case ASIN:
		if (abs(input) >= 1) throw MathDefinitionRangeException(GGE2305_OutsideGoniometricFunctionDefinitionRange, location, this->GetName(), input);
		result = asin(input);
		break;
	case ACOS:
		if (abs(input) >= 1) throw MathDefinitionRangeException(GGE2305_OutsideGoniometricFunctionDefinitionRange, location, this->GetName(), input);
		result = acos(input);
		break;
	case ATAN:
		result = atan(input);
		break;
	case DEG_TO_RAD:
		result = input * 180. / NUMBER_PI;
		break;
	case RAD_TO_DEG:
		result = input * NUMBER_PI / 180.;
		break;
	default:
		throw InternalErrorException("Unknown method.");
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}