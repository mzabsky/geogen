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

#include "CoordinateTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "CoordinateObject.hpp"
#include "CoordinateFromNumberFunctionDefinition.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

CoordinateTypeDefinition::CoordinateTypeDefinition() : TypeDefinition(GG_STR("Coordinate")) 
{
	this->GetStaticFunctionDefinitions().AddItem(new CoordinateFromNumberFunctionDefinition(this));
}

ManagedObject* CoordinateTypeDefinition::CreateInstance(VirtualMachine* vm, Number value, bool isRelative) const
{
	auto_ptr<ManagedObject> object(new CoordinateObject(vm, this, value, isRelative));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool CoordinateTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		CoordinateObject const* coordA = dynamic_cast<CoordinateObject const*>(a);
		CoordinateObject const* coordB = dynamic_cast<CoordinateObject const*>(b);
		return coordA->GetValue() < coordB->GetValue()/* && coordA->IsRelative() < coordB->IsRelative()*/;
	}
	
	return TypeDefinition::InstanceLessThan(a, b);
}

bool CoordinateTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<CoordinateObject const*>(a)->GetValue() == dynamic_cast<CoordinateObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* CoordinateTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return CreateInstance(vm, dynamic_cast<CoordinateObject const*>(a)->GetValue(), dynamic_cast<CoordinateObject const*>(a)->IsRelative());
}

bool CoordinateTypeDefinition::IsConvertibleFrom(VirtualMachine* vm, TypeDefinition const* anotherTypeDefinition) const
{
	return anotherTypeDefinition == this || anotherTypeDefinition == vm->GetNumberTypeDefinition();
}

ManagedObject* CoordinateTypeDefinition::Convert(VirtualMachine* vm, ManagedObject* object) const
{
	if (object->GetType() == this)
	{
		return object;
	}
	else if (object->GetType() == vm->GetNumberTypeDefinition())
	{
		NumberObject* numberObject = dynamic_cast<NumberObject*>(object);
		return this->CreateInstance(vm, numberObject->GetValue(), false);
	}
	else 
	{
		throw InternalErrorException(GG_STR("Invalid type conversion."));
	}
}