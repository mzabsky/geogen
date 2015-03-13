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

#include "RandomSequence2DGetNumberFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "RandomSequence2DTypeDefinition.hpp"
#include "PointTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "RandomSequence2DObject.hpp"
#include "PointObject.hpp"
#include "MaxLessThanMinException.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace random;

ManagedObject* RandomSequence2DGetNumberFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	PointTypeDefinition const* pointTypeDefinition = dynamic_cast<PointTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Point")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(pointTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 1);

	Point point = ((PointObject*)arguments[0])->GetAbsolutePoint(vm, location);

	RandomSequence2D& sequence = dynamic_cast<RandomSequence2DObject*>(instance)->GetSequence();

	Number n;
	if (arguments.size() > 1)
	{
		Number min = dynamic_cast<NumberObject*>(arguments[1])->GetValue();
		Number max = dynamic_cast<NumberObject*>(arguments[2])->GetValue();

		if (min > max)
		{
			throw MaxLessThanMinException(location);
		}

		n = sequence.GetDouble(point) * (max - min) + min;
	}
	else
	{
		n = sequence.GetDouble(point);
	}

	return numberTypeDefinition->CreateInstance(vm, n);
}