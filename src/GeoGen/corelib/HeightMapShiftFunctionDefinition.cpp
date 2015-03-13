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

#include "HeightMapShiftFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "SizeOverflowException.hpp"
#include "HeightMapShiftRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "DirectionTypeDefinition.hpp"
#include "CoordinateObject.hpp"
#include "../Direction.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapShiftFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	HeightProfileTypeDefinition const* heightProfileTypeDefinition = dynamic_cast<HeightProfileTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("HeightProfile")));
	DirectionTypeDefinition const* directionTypeDefinition = dynamic_cast<DirectionTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Direction")));
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(heightProfileTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(directionTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Direction direction = (Direction)NumberToInt(((NumberObject*)arguments[2])->GetValue());

	Number numberMaxDistance = ((CoordinateObject*)arguments[1])->GetAbsoluteCoordinate(vm, location, direction);
	Size1D maxDistance;
	if (!TryNumberToSize(numberMaxDistance, maxDistance))
	{
		throw SizeOverflowException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapShiftRenderingStep(location, argumentSlots, returnObjectSlot, maxDistance, direction);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}