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

#include "RandomSequence2DCreateFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "RandomSequence2DTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "RandomSequence2DObject.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;
using namespace geogen::renderer;
using namespace geogen::random;

ManagedObject* RandomSequence2DCreateFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	RandomSequence2DTypeDefinition const* sequenceTypeDefinition = dynamic_cast<RandomSequence2DTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("RandomSequence2D")));

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments, 0);

	RandomSeed passedSeed = 0;
	if (arguments.size() > 0)
	{
		passedSeed = (RandomSeed)NumberToInt(dynamic_cast<NumberObject*>(arguments[0])->GetValue());
	}
	else
	{
		passedSeed = (RandomSeed)vm->GetCommonRandomSequence().NextUInt();
	}
	
	RandomSeed actualSeed = CombineSeeds(passedSeed, vm->GetArguments().GetRandomSeed(), CreateSeed(GG_STR("RandomSequence2D")));

	return sequenceTypeDefinition->CreateInstance(vm, actualSeed);
}