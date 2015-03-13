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

#include "ScopeVariableReferenceObject.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;

ManagedObject* ScopeVariableReferenceObject::GetReferencedObject(CodeLocation location, VirtualMachine* vm)
{
	return this->variableTableItem->GetValue();
}

void ScopeVariableReferenceObject::SetReferencedObject(CodeLocation location, VirtualMachine* vm, ManagedObject* object)
{
	this->variableTableItem->SetValue(vm, object);
}

String ScopeVariableReferenceObject::GetStringValue() const
{
	return GG_STR("&[") + this->variableTableItem->GetValue()->GetStringValue() + GG_STR("]");
}