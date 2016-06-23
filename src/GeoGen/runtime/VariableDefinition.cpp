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

#include "../InternalErrorException.hpp"
#include "VariableDefinition.hpp"
#include "ManagedObject.hpp"
#include "VirtualMachine.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;


void VariableDefinition::Initialize(VirtualMachine* vm) const
{
	ManagedObject* defaultValue = this->CreateDefaultValue(vm);

	if (defaultValue != NULL)
	{
		vm->GetMemoryManager().RegisterObject(defaultValue);
	}

	if (!vm->GetGlobalVariableTable().DeclareVariable(this->GetName(), defaultValue, this->IsConstant()))
	{
		throw InternalErrorException(GG_STR("Could not declare static object variable (already initialized or name conflict?)."));
	}
}

void VariableDefinition::Serialize(IOStream& stream) const
{
	stream << (this->isConstant ? "const " : "") << this->name << endl;
}