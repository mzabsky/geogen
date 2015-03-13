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

#include "LogicalOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

LogicalOperatorFunctionDefinition* LogicalOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case LOGICAL_AND: return new LogicalOperatorFunctionDefinition(GG_STR("&&"), CallOperatorLogicalAnd);
	case LOGICAL_OR: return new LogicalOperatorFunctionDefinition(GG_STR("||"), CallOperatorLogicalOr);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* LogicalOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(booleanTypeDefinition);
	expectedTypes.push_back(booleanTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	BooleanObject* a = dynamic_cast<BooleanObject*>(arguments[0]);
	BooleanObject* b = dynamic_cast<BooleanObject*>(arguments[1]);

	bool result = this->function(location, a->GetValue(), b->GetValue());

	ManagedObject* returnObject = booleanTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

bool LogicalOperatorFunctionDefinition::CallOperatorLogicalAnd(CodeLocation location, bool a, bool b)
{
	return a && b;
}

bool LogicalOperatorFunctionDefinition::CallOperatorLogicalOr(CodeLocation location, bool a, bool b)
{
	return a || b;
}