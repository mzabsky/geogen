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

#include "ArrayRemoveFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ArrayRemoveFunctionDefinition* ArrayRemoveFunctionDefinition::Create(Method method, runtime::TypeDefinition const* owningType)
{
	switch (method)
	{
	case REMOVE_KEY:
		return new ArrayRemoveFunctionDefinition(GG_STR("RemoveKey"), method, owningType);
	case REMOVE_VALUE:
		return new ArrayRemoveFunctionDefinition(GG_STR("RemoveValue"), method, owningType);
	default: throw InternalErrorException(GG_STR("Unknown method."));
	}
}

ManagedObject* ArrayRemoveFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{
	this->CheckArguments(location, 1, arguments);

	ArrayObject* thisArray = dynamic_cast<ArrayObject*>(instance);

	switch (this->method)
	{
	case REMOVE_KEY:
		thisArray->RemoveKey(vm, location, arguments[0]);
		break;
	case REMOVE_VALUE:
		thisArray->RemoveValue(vm, location, arguments[0]);
		break;
	default:
		throw InternalErrorException(GG_STR("Unknown method."));
	}

	return vm->GetNull();
}