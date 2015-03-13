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

#include "MathSqrtFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "MathDefinitionRangeException.hpp"
#include "../runtime/NumberUnderflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* MathSqrtFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameters;
	expectedParameters.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameters, arguments);

	Number input = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	if (input < 0) throw MathDefinitionRangeException(GGE2307_OutsideSqrtFunctionDefinitionRange, location, this->GetName(), input);

	Number result = sqrt(input);

	if (input != 0 && result == 0)
	{
		throw NumberUnderflowException(location);
	}

	return numberTypeDefinition->CreateInstance(vm, result);
}