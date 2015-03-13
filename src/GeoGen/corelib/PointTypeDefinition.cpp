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

#include "PointTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "CoordinateTypeDefinition.hpp"
#include "PointObject.hpp"
#include "PointCreateFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

PointTypeDefinition::PointTypeDefinition() : TypeDefinition(GG_STR("Point"))
{
	this->GetStaticFunctionDefinitions().AddItem(new PointCreateFunctionDefinition(this));
}

ManagedObject* PointTypeDefinition::CreateInstance(VirtualMachine* vm, Number x, Number y, bool isXRelative, bool isYRelative) const
{
	CoordinateTypeDefinition const* coordinateTypeDefinition = dynamic_cast<CoordinateTypeDefinition const*>(vm->GetTypeDefinition(GG_STR("Coordinate")));

	auto_ptr<ManagedObject> object(new PointObject(vm, this, x, y, isXRelative, isYRelative));
	vm->GetMemoryManager().RegisterObject(object.get());

	object->GetMemberVariableTable().DeclareVariable(GG_STR("X"), coordinateTypeDefinition->CreateInstance(vm, x, isXRelative), true);
	object->GetMemberVariableTable().DeclareVariable(GG_STR("Y"), coordinateTypeDefinition->CreateInstance(vm, y, isYRelative), true);

	return object.release();
}

bool PointTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		PointObject const* pointA = dynamic_cast<PointObject const*>(a);
		PointObject const* pointB = dynamic_cast<PointObject const*>(b);
		
		return pointA->GetX() < pointB->GetX() && pointA->GetY() < pointB->GetY() && pointA->IsXRelative() < pointB->IsXRelative() && pointA->IsYRelative() < pointB->IsYRelative();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool PointTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		PointObject const* pointA = dynamic_cast<PointObject const*>(a);
		PointObject const* pointB = dynamic_cast<PointObject const*>(b);

		return pointA->GetX() == pointB->GetX() && pointA->GetY() == pointB->GetY() && pointA->IsXRelative() == pointB->IsXRelative() && pointA->IsYRelative() == pointB->IsYRelative();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* PointTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	PointObject const* original = dynamic_cast<PointObject const*>(a);	
	return CreateInstance(vm, original->GetX(), original->GetY(), original->IsXRelative(), original->IsYRelative());
}