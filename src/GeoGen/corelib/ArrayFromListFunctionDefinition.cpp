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

#include "ArrayFromListFunctionDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ArrayTypeDefinition.hpp"
#include "ArrayObject.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::corelib;
using namespace geogen::runtime;

ManagedObject* ArrayFromListFunctionDefinition::CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, vector<ManagedObject*> arguments) const
{	
	// Make sure the number of arguments is even
	this->CheckArguments(location, arguments.size() - arguments.size() % 2, arguments);

	ArrayTypeDefinition const* arrayTypeDefinition = dynamic_cast<ArrayTypeDefinition const*>(instance->GetType());
	ArrayObject* returnObject = dynamic_cast<ArrayObject*>(arrayTypeDefinition->CreateInstance(vm));

	for (unsigned i = 0; i < arguments.size() / 2; i++)
	{
		ManagedObject* key = arguments[unsigned(i * 2 + 1)];
		ManagedObject* value = arguments[unsigned(i * 2)];

		VariableTableItem* unsetKeyVariable = vm->FindVariable(ArrayTypeDefinition::UNSET_KEY_VARIABLE_NAME);
		if (unsetKeyVariable == NULL)
		{
			throw InternalErrorException(GG_STR("Could not find unset key marker."));
		}

		if (key == unsetKeyVariable->GetValue())
		{
			returnObject->PushBack(vm, location, value);
		}
		else
		{
			returnObject->Set(vm, location, key, value);
		}
	}

	return returnObject;
}