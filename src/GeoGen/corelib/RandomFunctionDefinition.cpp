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

#include "RandomFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "MaxLessThanMinException.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* RandomFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedParameters;
	if (arguments.size() > 0)
	{
		expectedParameters.push_back(numberTypeDefinition);
		expectedParameters.push_back(numberTypeDefinition);
	}
	
	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedParameters, arguments);

	Number n;
	if (arguments.size() > 0)
	{
		Number minValue = dynamic_cast<NumberObject*>(arguments[0])->GetValue();
		Number maxValue = dynamic_cast<NumberObject*>(arguments[1])->GetValue();

		if (minValue > maxValue)
		{
			throw MaxLessThanMinException(location);
		}

		n = vm->GetCommonRandomSequence().NextDouble() * (maxValue - minValue) + minValue;
	}
	else 
	{
		n = vm->GetCommonRandomSequence().NextDouble();
	}

	return numberTypeDefinition->CreateInstance(vm, n);
}