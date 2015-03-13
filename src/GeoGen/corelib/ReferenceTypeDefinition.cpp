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

#include <memory>

#include "../InternalErrorException.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "ReferenceTypeDefinition.hpp"
#include "ScopeVariableReferenceObject.hpp"
#include "ArrayItemReferenceObject.hpp"
#include "ArrayObject.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;

ManagedObject* ReferenceTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	return a;
}

ManagedObject* ReferenceTypeDefinition::CreateScopeReferenceInstance(VirtualMachine* vm, VariableTableItem* variableTableItem) const
{
	auto_ptr<ManagedObject> object(new ScopeVariableReferenceObject(vm, this, variableTableItem));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

ManagedObject* ReferenceTypeDefinition::CreateArrayItemReferenceInstance(VirtualMachine* vm, ArrayObject* array, ManagedObject* key) const
{
	auto_ptr<ManagedObject> object(new ArrayItemReferenceObject(vm, this, array, key));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}