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

#include "HeightMapStratifyFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapStratifyRenderingStep.hpp"
#include "BooleanTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "InvalidStrengthException.hpp"
#include "InvalidNumberOfStrataException.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace renderer;

ManagedObject* HeightMapStratifyFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(booleanTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	int numberOfStrata = dynamic_cast<NumberObject*>(arguments[0])->GetValue();

	if (numberOfStrata < 1 || numberOfStrata > 100)
	{
		throw InvalidNumberOfStrataException(location);
	}

	Number steepness = arguments.size() > 1 ? dynamic_cast<NumberObject*>(arguments[1])->GetValue() : 0.5;

	if (steepness < 0 || steepness > 1)
	{
		throw InvalidStrengthException(location);
	}

	Number smoothness = arguments.size() > 2 ? dynamic_cast<NumberObject*>(arguments[2])->GetValue() : 0.5;

	if (smoothness < 0 || smoothness > 1)
	{
		throw InvalidStrengthException(location);
	}

	bool includeNegative = arguments.size() > 3 ? dynamic_cast<BooleanObject*>(arguments[3])->GetValue() : false;

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapStratifyRenderingStep(location, argumentSlots, returnObjectSlot, numberOfStrata, steepness, smoothness, includeNegative);
	vm->GetRenderingSequence().AddStep(renderingStep);

	return instance;
}