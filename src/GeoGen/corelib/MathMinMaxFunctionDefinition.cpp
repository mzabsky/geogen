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

#include "MathMinMaxFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

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

	int parameterCount = std::max(static_cast<size_t>(2u), arguments.size());
	vector<TypeDefinition const*> expectedParameterTypes;

	for (int i = 0; i < parameterCount; i++)
	{
		expectedParameterTypes.push_back(numberTypeDefinition);
	}

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameterTypes, arguments);

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