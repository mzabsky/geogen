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

#include "ArrayItemReferenceObject.hpp"
#include "ArrayObject.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace runtime;

ManagedObject* ArrayItemReferenceObject::GetReferencedObject(CodeLocation location, VirtualMachine* vm)
{
	return this->array->Get(vm, location, this->key);
}

void ArrayItemReferenceObject::SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object)
{
	this->array->Set(vm, location, this->key, object);
}

String ArrayItemReferenceObject::GetStringValue() const
{
	StringStream ss;
	ss << GG_STR("&[array #") << this->array->GetObjectId() << ", key #" << this->key->GetObjectId() << GG_STR("]");
	return ss.str();
}