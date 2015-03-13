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

#include "../CodeLocation.hpp"
#include "ManagedObject.hpp"
#include "ReadOnlyWriteException.hpp"
#include "UndefinedSymbolAccessException.hpp"
#include "MemoryManager.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"
#include "..\InternalErrorException.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ManagedObject::ManagedObject(VirtualMachine* vm, TypeDefinition const* type) : type(type), memberVariableTable(&vm->GetMemoryManager()), objectId(UNASSIGNED_OBJECT_ID), refCount(0)
{	
	/*for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = type->GetVariableDefinitions().)
	for
	this->GetType()*/
}

ManagedObject::~ManagedObject()
{
#ifdef DEBUG				
	if (this->magic != MANAGED_OBJECT_MAGIC)
	{
		throw InternalErrorException(GG_STR("Memory corruption detected when deleting managed object (object already deleted?)"));
	}
#endif
};

void ManagedObject::AddRef(/*MemoryManager& vm*/)
{
	if (this->GetObjectId() == 0) throw exception();

	this->refCount++;
}

void ManagedObject::RemoveRef(MemoryManager& vm)
{
	if (this->refCount == 0)
	{
		throw InternalErrorException(GG_STR("Can't reduce refcount of an object with no references."));
	}

	this->refCount--;

	if (this->refCount == 0)
	{
		vm.DestroyObject(this);
	}
}

void ManagedObject::Serialize(IOStream& stream) const
{
	stream << "#" << this->objectId << " " << this->type->GetName() << " {" << this->GetStringValue() << "}";
	if (this->memberVariableTable.Size() > 0)
	{
		stream << endl;
		this->memberVariableTable.SerializeWithTabs(stream, 1);
	}
}
