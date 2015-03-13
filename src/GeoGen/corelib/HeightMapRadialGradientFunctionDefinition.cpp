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

#include "HeightMapRadialGradientFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "NumberTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "PointObject.hpp"
#include "HeightMapTypeDefinition.hpp"
#include "HeightMapRadialGradientRenderingStep.hpp"
#include "HeightOverflowException.hpp"
#include "SizeOverflowException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;

ManagedObject* HeightMapRadialGradientFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	Point point = ((PointObject*)arguments[0])->GetAbsolutePoint(vm, location);
	Number numberRadius = ((NumberObject*)arguments[1])->GetValue();
	Number numberFromHeight = ((NumberObject*)arguments[2])->GetValue();
	Number numberToHeight = ((NumberObject*)arguments[3])->GetValue();
	
	Size1D radius;
	if (!TryNumberToSize(numberRadius, radius))
	{
		throw SizeOverflowException(location);
	}

	Height fromHeight;
	if (!TryNumberToHeight(numberFromHeight, fromHeight))
	{
		throw HeightOverflowException(location);
	}

	Height toHeight;
	if (!TryNumberToHeight(numberToHeight, toHeight))
	{
		throw HeightOverflowException(location);
	}

	ManagedObject* returnObject = dynamic_cast<HeightMapTypeDefinition const*>(instance->GetType())->CreateInstance(vm);

	vector<unsigned> argumentSlots;
	unsigned returnObjectSlot = vm->GetRendererObjectSlotTable().GetObjectSlotByAddress(returnObject);
	RenderingStep* renderingStep = new HeightMapRadialGradientRenderingStep(location, argumentSlots, returnObjectSlot, point, radius, fromHeight, toHeight);
	vm->AddRenderingStep(location, renderingStep);

	return returnObject;
}