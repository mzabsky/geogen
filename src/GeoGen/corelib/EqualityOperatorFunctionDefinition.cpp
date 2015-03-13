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

#include "EqualityOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

EqualityOperatorFunctionDefinition* EqualityOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case EQUAL_TO: return new EqualityOperatorFunctionDefinition(GG_STR("=="), CallOperatorEqualTo);
	case NOT_EQUAL_TO: return new EqualityOperatorFunctionDefinition(GG_STR("!="), CallOperatorNotEqualTo);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* EqualityOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	this->CheckArguments(location, 2, arguments);

	bool result = this->function(location, arguments[0], arguments[1]);

	ManagedObject* returnObject = booleanTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

bool EqualityOperatorFunctionDefinition::CallOperatorEqualTo(CodeLocation location, ManagedObject* a, ManagedObject* b)
{
	return a->GetType()->InstanceEqualsTo(a, b);
}

bool EqualityOperatorFunctionDefinition::CallOperatorNotEqualTo(CodeLocation location, ManagedObject* a, ManagedObject* b)
{
	return !a->GetType()->InstanceEqualsTo(a, b);
}