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

#include "PointCreateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "RelativeCoordinateInInfiniteDimensionException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* PointCreateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	PointTypeDefinition const* coordinateType = dynamic_cast<PointTypeDefinition const*>(this->GetOwningTypeDefinition());

	vector<TypeDefinition const*> expectedTypes;	
	expectedTypes.push_back(coordinateTypeDefinition);	
	expectedTypes.push_back(coordinateTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	CoordinateObject* coordX = dynamic_cast<CoordinateObject*>(arguments[0]);
	CoordinateObject* coordY = dynamic_cast<CoordinateObject*>(arguments[1]);
	
	if ((coordX->IsRelative() && vm->GetArguments().GetMapWidth() == MAP_SIZE_INFINITE) || (coordY->IsRelative() && vm->GetArguments().GetMapHeight() == MAP_SIZE_INFINITE))
	{
		throw RelativeCoordinateInInfiniteDimensionException(location);
	}

	return coordinateType->CreateInstance(vm, coordX->GetValue(), coordY->GetValue(), coordX->IsRelative(), coordY->IsRelative());
}