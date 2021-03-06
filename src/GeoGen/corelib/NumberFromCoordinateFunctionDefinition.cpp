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

#include "NumberFromCoordinateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* NumberFromCoordinateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	TypeDefinition const* directionTypeDefinition = vm->GetTypeDefinition(GG_STR("Direction"));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	CoordinateObject* coordinateValue = dynamic_cast<CoordinateObject*>(arguments[0]);

	if (coordinateValue->IsRelative())
	{
		// Direction has to be supplied for relative coordinate.
		if (arguments.size() < 2)
		{
			throw UnknownRelativeCoordinateDirectionException(location);
		}

		Direction direction = (Direction)NumberToInt(dynamic_cast<NumberObject*>(arguments[1])->GetValue());

		return numberTypeDefinition->CreateInstance(vm, coordinateValue->GetAbsoluteCoordinate(vm, location, direction));
	}
	else
	{
		return numberTypeDefinition->CreateInstance(vm, coordinateValue->GetValue());
	}
}