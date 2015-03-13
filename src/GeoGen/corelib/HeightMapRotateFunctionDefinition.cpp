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

#include "HeightMapRotateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapFlatRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "HeightMapTransformRenderingStep.hpp"
#include "NumberTypeDefinition.hpp"
#include "InvalidTransformationMatrixException.hpp"
#include "../genlib/TransformationMatrix.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace renderer;
using namespace genlib;

ManagedObject* HeightMapRotateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	double angle = ((NumberObject*)arguments[0])->GetValue();

	TransformationMatrix matrix;
	matrix.A11 = cos(angle);
	matrix.A12 = -sin(angle);
	matrix.A21 = sin(angle);
	matrix.A22 = cos(angle);

	if (!matrix.IsInvertible())
	{
		throw InvalidTransformationMatrixException(location);
	}

	vector<unsigned> argumentSlots;
	argumentSlots.push_back(vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance));
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(instance);
	RenderingStep* renderingStep = new HeightMapTransformRenderingStep(location, argumentSlots, returnObjectSlot, matrix);
	vm->AddRenderingStep(location, renderingStep);

	return instance;
}