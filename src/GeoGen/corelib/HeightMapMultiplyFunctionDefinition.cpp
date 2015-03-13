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

#include "HeightMapMultiplyFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapMultiplyRenderingStep.hpp"
#include "HeightMapMultiplyMapRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapMultiplyFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;

	bool isNumberMode;
	if (arguments.size() > 0 && arguments[0]->GetType() == this->GetOwningTypeDefinition())
	{
		isNumberMode = false;
		expectedTypes.push_back(this->GetOwningTypeDefinition());
	}
	else
	{
		isNumberMode = true;
		expectedTypes.push_back(numberTypeDefinition);
	}

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));

	if (isNumberMode)
	{
		Number number = ((NumberObject*)arguments[0])->GetValue();

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightMapMultiplyRenderingStep(location, argumentSlots, returnObjectSlot, number);
		vm->AddRenderingStep(location, renderingStep);
	}
	else
	{
		argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(arguments[0]));

		unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
		RenderingStep* renderingStep = new HeightMapMultiplyMapRenderingStep(location, argumentSlots, returnObjectSlot);
		vm->AddRenderingStep(location, renderingStep);
	}

	return instance;
}