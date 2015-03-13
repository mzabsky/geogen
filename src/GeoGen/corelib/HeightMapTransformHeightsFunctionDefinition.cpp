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

#include "HeightMapTransformHeightsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "SizeOverflowException.hpp"
#include "CoordinateObject.hpp"
#include "HeightMapTransformHeightsRenderingStep.hpp"
#include "UnknownRelativeCoordinateDirectionException.hpp"
#include "MaxLessThanMinException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapTransformHeightsFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	HeightProfileTypeDefinition const* heightProfileTypeDefinition = dynamic_cast<HeightProfileTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("HeightProfile")));
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(heightProfileTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(coordinateTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	CoordinateObject* startObject = arguments.size() > 1 ? dynamic_cast<CoordinateObject*>(arguments[1]) : NULL;
	CoordinateObject* endObject = arguments.size() > 2 ? dynamic_cast<CoordinateObject*>(arguments[2]) : NULL;

	if ((startObject != NULL && startObject->IsRelative()) || (endObject != NULL && endObject->IsRelative()))
	{
		throw UnknownRelativeCoordinateDirectionException(location);
	}

	Coordinate intervalStart = arguments.size() > 1 ? startObject->GetValue(): 0;
	Coordinate intervalEnd = arguments.size() > 2 ? endObject->GetValue(): 100;

	Number numberMinHeight = arguments.size() > 3 ? ((NumberObject*)arguments[3])->GetValue() : 0;
	Height minHeight;
	if (!TryNumberToHeight(numberMinHeight, minHeight))
	{
		throw HeightOverflowException(location);
	}

	Number numberMaxHeight = arguments.size() > 4 ? ((NumberObject*)arguments[4])->GetValue() : 1;
	Height maxHeight;
	if (!TryNumberToHeight(numberMaxHeight, maxHeight))
	{
		throw HeightOverflowException(location);
	}

	if (minHeight > maxHeight)
	{
		throw MaxLessThanMinException(location);
	}

	vector<unsigned> argumentSlots;	
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapTransformHeightsRenderingStep(location, argumentSlots, returnObjectSlot, Interval::FromCoordinates(intervalStart, intervalEnd), minHeight, maxHeight);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}