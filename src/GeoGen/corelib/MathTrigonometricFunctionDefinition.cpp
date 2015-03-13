/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <cmath>

#include "MathTrigonometricFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"
#include "../runtime/NumberOverflowException.hpp"

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

	Number result;
	switch (this->function)
	{
	case SIN:
		result = std::sin(input);
		break;
	case COS:
		result = std::cos(input);
		break;
	case TAN:
		result = std::tan(input);
		break;
	case ASIN:
		if (std::abs(input) >= 1) throw MathDefinitionRangeException(GGE2305_OutsideTrigonometricFunctionDefinitionRange, location, this->GetName(), input);
		result = std::asin(input);
		break;
	case ACOS:
		if (std::abs(input) >= 1) throw MathDefinitionRangeException(GGE2305_OutsideTrigonometricFunctionDefinitionRange, location, this->GetName(), input);
		result = std::acos(input);
		break;
	case ATAN:
		result = std::atan(input);
		break;
	case DEG_TO_RAD:
		result = input * NUMBER_PI / 180.;

		if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
		{
			throw NumberOverflowException(location);
		}
		break;
	case RAD_TO_DEG:
		result = input * 180. / NUMBER_PI;

		if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
		{
			throw NumberOverflowException(location);
		}
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}