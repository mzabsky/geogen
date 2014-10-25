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
		return new MathTrigonometricFunctionDefinition(GG_STR("Sin"), function);
	case COS:
		return new MathTrigonometricFunctionDefinition(GG_STR("Cos"), function);
	case TAN:
		return new MathTrigonometricFunctionDefinition(GG_STR("Tan"), function);
	case ASIN:
		return new MathTrigonometricFunctionDefinition(GG_STR("Asin"), function);
	case ACOS:
		return new MathTrigonometricFunctionDefinition(GG_STR("Acos"), function);
	case ATAN:
		return new MathTrigonometricFunctionDefinition(GG_STR("Atan"), function);
	case DEG_TO_RAD:
		return new MathTrigonometricFunctionDefinition(GG_STR("DegToRad"), function);
	case RAD_TO_DEG:
		return new MathTrigonometricFunctionDefinition(GG_STR("RadToDeg"), function);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* MathTrigonometricFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameterTypes;
	expectedParameterTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameterTypes, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	RuntimeMathCheckInit();

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
		if (abs(input) >= 1) throw MathDefinitionRangeException(GGE2305_OutsideTrigonometricFunctionDefinitionRange, location, this->GetName(), input);
		result = asin(input);
		break;
	case ACOS:
		if (abs(input) >= 1) throw MathDefinitionRangeException(GGE2305_OutsideTrigonometricFunctionDefinitionRange, location, this->GetName(), input);
		result = acos(input);
		break;
	case ATAN:
		result = atan(input);
		break;
	case DEG_TO_RAD:
		result = input * NUMBER_PI / 180.;
		break;
	case RAD_TO_DEG:
		result = input * 180. / NUMBER_PI;
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	RuntimeMathCheck(location);

	return numberTypeDefinition->CreateInstance(vm, result);
}