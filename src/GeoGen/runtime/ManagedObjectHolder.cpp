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

#include "ManagedObjectHolder.hpp"
#include "ManagedObject.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

ManagedObjectHolder::ManagedObjectHolder(VirtualMachine* vm, ManagedObject* object) : vm(vm), object(object) 
{
	this->object->AddRef();
};

ManagedObjectHolder::ManagedObjectHolder(ManagedObjectHolder const& other) 
{
	this->vm = other.vm;
	this->object = other.object;
	this->object->AddRef();
}

ManagedObjectHolder& ManagedObjectHolder::operator=(ManagedObjectHolder const& other)
{
	if (this->object != NULL)
	{
		this->object->RemoveRef(this->vm->GetMemoryManager());
	}

	this->vm = other.vm;
	this->object = other.object;
	this->object->AddRef();

	return *this;
}

ManagedObjectHolder::~ManagedObjectHolder()
{
	if (this->object != NULL)
	{
		this->object->RemoveRef(vm->GetMemoryManager());
	}
}