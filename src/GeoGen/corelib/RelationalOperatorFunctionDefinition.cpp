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

#include "RelationalOperatorFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

RelationalOperatorFunctionDefinition* RelationalOperatorFunctionDefinition::Create(Operator op)
{
	switch (op)
	{
	case LESS_THAN: return new RelationalOperatorFunctionDefinition(GG_STR("<"), CallOperatorLessThan);
	case LESS_THAN_OR_EQUAL_TO: return new RelationalOperatorFunctionDefinition(GG_STR("<="), CallOperatorLessThanOrEqualTo);
	case GREATER_THAN: return new RelationalOperatorFunctionDefinition(GG_STR(">"), CallOperatorGreaterThan);
	case GREATER_THAN_OR_EQUAL_TO: return new RelationalOperatorFunctionDefinition(GG_STR(">="), CallOperatorGreaterThanOrEqualTo);
	default: throw InternalErrorException(GG_STR("Unknown operator type."));
	}
}

ManagedObject* RelationalOperatorFunctionDefinition::CallNative(CodeLocation location, VirtualMachine* vm, vector<ManagedObject*> arguments) const
{
	NumberTypeDefinition const* numberTypeDefinition = vm->GetNumberTypeDefinition();
	BooleanTypeDefinition const* booleanTypeDefinition = vm->GetBooleanTypeDefinition();

	vector<TypeDefinition const*> expectedTypes;
	expectedTypes.push_back(numberTypeDefinition);
	expectedTypes.push_back(numberTypeDefinition);

	vector<ManagedObjectHolder> convertedObjectHolders = this->CheckArguments(vm, location, expectedTypes, arguments);

	NumberObject* a = dynamic_cast<NumberObject*>(arguments[0]);
	NumberObject* b = dynamic_cast<NumberObject*>(arguments[1]);

	bool result = this->function(location, a->GetValue(), b->GetValue());

	ManagedObject* returnObject = booleanTypeDefinition->CreateInstance(vm, result);
	return returnObject;
}

bool RelationalOperatorFunctionDefinition::CallOperatorLessThan(CodeLocation location, Number a, Number b)
{
	return a < b;
}

bool RelationalOperatorFunctionDefinition::CallOperatorLessThanOrEqualTo(CodeLocation location, Number a, Number b)
{
	return a <= b;
}

bool RelationalOperatorFunctionDefinition::CallOperatorGreaterThan(CodeLocation location, Number a, Number b)
{
	return a > b;
}

bool RelationalOperatorFunctionDefinition::CallOperatorGreaterThanOrEqualTo(CodeLocation location, Number a, Number b)
{
	return a >= b;
}