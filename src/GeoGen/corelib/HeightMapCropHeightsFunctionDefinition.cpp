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

#include "HeightMapCropHeightsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapCropHeightsRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "MaxLessThanMinException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapCropHeightsFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 2);

	Number numberMin = ((NumberObject*)arguments[0])->GetValue();
	Height min;
	if (!TryNumberToHeight(numberMin, min))
	{
		throw HeightOverflowException(location);
	}

	Number numberMax = ((NumberObject*)arguments[1])->GetValue();
	Height max;
	if (!TryNumberToHeight(numberMax, max))
	{
		throw HeightOverflowException(location);
	}

	Number numberReplace = arguments.size() > 2 ? ((NumberObject*)arguments[2])->GetValue() : 0;
	Height replace;
	if (!TryNumberToHeight(numberReplace, replace))
	{
		throw HeightOverflowException(location);
	}

	if (min > max)
	{
		throw MaxLessThanMinException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapCropHeightsRenderingStep(location, argumentSlots, returnObjectSlot, min, max, replace);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}