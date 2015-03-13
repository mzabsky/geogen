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

#include "HeightProfileFlatFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "..\runtime\IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "HeightProfileTypeDefinition.hpp"
#include "HeightProfileFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightProfileFlatFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	Number numberHeight = arguments.size() > 0 ? ((NumberObject*)arguments[0])->GetValue() : 0;
	Height height;
	if (!TryNumberToHeight(numberHeight, height))
	{
		throw HeightOverflowException(location);
	}

	ManagedObject* returnObject = dynamic_cast<HeightProfileTypeDefinition const*>(instance->GetType())->CreateInstance(vm);
	
	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightProfileFlatRenderingStep(location, argumentSlots, returnObjectSlot, height);
	vm->AddRenderingStep(location, renderingStep);

	return returnObject;
} 