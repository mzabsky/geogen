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

#include "HeightMapAddFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapAddRenderingStep.hpp"
#include "HeightMapAddMapRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapAddFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
    
    vector<TypeDefinition const*> expectedTypes;

	bool isHeightMode;
	if (arguments.size() > 0 && arguments[0]->GetType() == this->GetOwningTypeDefinition())
	{
		isHeightMode = false;
		expectedTypes.push_back(this->GetOwningTypeDefinition());
	}	
	else 
    {
		isHeightMode = true;
		expectedTypes.push_back(numberTypeDefinition);
	}

	expectedTypes.push_back(this->GetOwningTypeDefinition()); // mask

	bool hasMask = arguments.size() > 1;

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));

	if (isHeightMode)
	{
		if (hasMask)
		{
			argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[1]));
		}

		Number numberHeight = ((NumberObject*)arguments[0])->GetValue();
		Height height;
		if (!TryNumberToHeight(numberHeight, height))
		{
			throw HeightOverflowException(location);
		}

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightMapAddRenderingStep(location, argumentSlots, returnObjectSlot, height);
		vm->AddRenderingStep(location, renderingStep);
	}
	else 
    {
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));

		if (hasMask)
		{
			argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[1]));
		}

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightMapAddMapRenderingStep(location, argumentSlots, returnObjectSlot);
		vm->AddRenderingStep(location, renderingStep);
	}

	return instance;
}