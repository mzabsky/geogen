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

#include "HeightMapCropFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointObject.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "SizeOverflowException.hpp"
#include "HeightMapCropRenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapCropFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 2);

	Point fromPoint = dynamic_cast<PointObject*>(arguments[0])->GetAbsolutePoint(vm, location);
	Point toPoint = dynamic_cast<PointObject*>(arguments[1])->GetAbsolutePoint(vm, location);

	Number heightNumber = arguments.size() > 2 ? dynamic_cast<NumberObject*>(arguments[2])->GetValue() : 0;
	Height height;
	if (!TryNumberToHeight(heightNumber, height))
	{
		throw HeightOverflowException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapCropRenderingStep(location, argumentSlots, returnObjectSlot, Rectangle(fromPoint, toPoint), height);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}