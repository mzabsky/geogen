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

#include "ArrayContainsFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../corelib/BooleanTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ArrayContainsFunctionDefinition* ArrayContainsFunctionDefinition::Create(Method method, runtime::TypeDefinition const* owningType)
{
	switch (method)
	{
	case CONTAINS_KEY:
		return new ArrayContainsFunctionDefinition(GG_STR("ContainsKey"), method, owningType);
	case CONTAINS_VALUE:
		return new ArrayContainsFunctionDefinition(GG_STR("ContainsValue"), method, owningType);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* ArrayContainsFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 1, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	bool result;
	switch (this->method)
	{
	case CONTAINS_KEY:
		result = thisArray->ContainsKey(vm, location, arguments[0]);
		break;
	case CONTAINS_VALUE:
		result = thisArray->ContainsValue(vm, location, arguments[0]);
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	BooleanTypeDefinition const* boolTypeDefinition = vm->GetBooleanTypeDefinition();
	return boolTypeDefinition->CreateInstance(vm, result);
}