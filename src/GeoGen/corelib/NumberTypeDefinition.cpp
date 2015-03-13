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

#include "NumberTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../Number.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "NumberFromEnumFunctionDefinition.hpp"
#include "NumberFromCoordinateFunctionDefinition.hpp"

using namespace geogen;
using namespace runtime;
using namespace corelib;
using namespace std;

NumberTypeDefinition::NumberTypeDefinition() : TypeDefinition(GG_STR("Number")) 
{
	this->GetStaticFunctionDefinitions().AddItem(new NumberFromEnumFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new NumberFromCoordinateFunctionDefinition(this));
}

ManagedObject* NumberTypeDefinition::CreateInstance(VirtualMachine* vm, Number value) const
{
	auto_ptr<ManagedObject> object(new NumberObject(vm, this, value));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool NumberTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<NumberObject const*>(a)->GetValue() < dynamic_cast<NumberObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool NumberTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<NumberObject const*>(a)->GetValue() == dynamic_cast<NumberObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* NumberTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

    // Value type, copies
	return CreateInstance(vm, static_cast<NumberObject const*>(a)->GetValue());
}