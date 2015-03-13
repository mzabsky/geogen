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

#include "MathLogPowFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "..\InternalErrorException.hpp"
#include "MathDefinitionRangeException.hpp"
#include "../runtime/NumberOverflowException.hpp"
#include "../runtime/NumberUnderflowException.hpp"

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

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameterTypes, arguments);

	Number arg1 = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	Number arg2 = dynamic_cast<NumberObject*>(arguments[1])->GetValue();

	Number result;
	switch (this->function)
	{
	case LOG:
		if (arg1 <= 0 || arg2 <= 0) throw MathDefinitionRangeException(GGE2306_OutsideLogFunctionDefinitionRange, location, this->GetName(), arg1);
		result = log(arg1) / log(arg2);

		if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
		{
			throw NumberOverflowException(location);
		}
		else if (result == 0)
		{
			throw NumberUnderflowException(location);
		}
		break;
	case POW:
		result = pow(arg1, arg2);

		if (result == numeric_limits<Number>::infinity() || result == -numeric_limits<Number>::infinity())
		{
			throw NumberOverflowException(location);
		}
		else if (arg1 != 0 && result == 0)
		{
			throw NumberUnderflowException(location);
		}
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}