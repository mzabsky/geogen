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

#include "IndexerWriteFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/NumberOfArgumentsException.hpp"
#include "../InternalErrorException.hpp"
#include "..\runtime\IncorrectTypeException.hpp"
#include "NumberTypeDefinition.hpp"
#include "BooleanTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* IndexerWriteFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();

	this->CheckArguments(location, 1, arguments);

	if (arguments[0]->IsStaticObject())
	{
		throw IncorrectTypeException(GGE2102_IncorrectOperandType, location, numberTypeDefinition->GetName(), GG_STR("Type"));
	}

	ManagedObject* returnObject;
	/*if (arguments[0]->GetType() == numberTypeDefinition)
	{
	int value = (int)dynamic_cast<NumberObject*>(arguments[0])->GetValue();
	returnObject = numberTypeDefinition->CreateInstance(!(int)value);
	}
	else */if (arguments[0]->GetType() == booleanTypeDefinition){
		bool value = dynamic_cast<BooleanObject*>(arguments[0])->GetValue();
		returnObject = booleanTypeDefinition->CreateInstance(vm, !value);
	}
	else
	{
		throw IncorrectTypeException(GGE2102_IncorrectOperandType, location, booleanTypeDefinition->GetName(), arguments[0]->GetType()->GetName());
	}

	return returnObject;
}