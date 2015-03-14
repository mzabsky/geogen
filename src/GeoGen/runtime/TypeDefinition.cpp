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
#include "TypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "StaticObject.hpp"
#include "VirtualMachine.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

void TypeDefinition::Initialize(VirtualMachine* vm) const
{
	ManagedObject* staticInstance = this->CreateStaticObject(vm);

	if (!vm->GetGlobalVariableTable().DeclareVariable(this->GetName(), staticInstance, true))
	{
		throw InternalErrorException(GG_STR("Could not declare static object variable (already initialized or name conflict?)."));
	}
}

StaticObject* TypeDefinition::CreateStaticObject(VirtualMachine* vm) const
{
	auto_ptr<StaticObject> object (new StaticObject(vm, this));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool TypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != b->GetType())
	{
		return a->GetType()->GetName() < b->GetType()->GetName();
	}
	
	return a->GetObjectId() < b->GetObjectId();
}

bool TypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	return a == b;
}

bool TypeDefinition::IsConvertibleFrom(VirtualMachine* vm, TypeDefinition const* anotherTypeDefinition) const
{
	return this == anotherTypeDefinition;
}

ManagedObject* TypeDefinition::Convert(VirtualMachine* vm, ManagedObject* object) const
{
	if (this == object->GetType())
	{
		return object;
	}
	else 
	{
		throw InternalErrorException(GG_STR("Invalid type conversion."));
	}
}

void TypeDefinition::Serialize(IOStream& stream) const
{
	//stream << "{" << endl;

	if (this->GetFunctionDefinitions().Size() > 0)
	{
		stream << GG_STR("FunctionDefinitions: ") << endl;
		this->GetFunctionDefinitions().SerializeWithTabs(stream, 1);
	}

	if (this->GetStaticFunctionDefinitions().Size() > 0)
	{
		stream << GG_STR("StaticFunctionDefinitions: ") << endl;
		this->GetStaticFunctionDefinitions().SerializeWithTabs(stream, 1);
	}

	if (this->GetVariableDefinitions().Size() > 0)
	{
		stream << GG_STR("VariableDefinitions: ") << endl;
		this->GetVariableDefinitions().SerializeWithTabs(stream, 1);
	}

	if (this->GetStaticVariableDefinitions().Size() > 0)
	{
		stream << GG_STR("StaticVariableDefinitions: ") << endl;
		this->GetStaticVariableDefinitions().SerializeWithTabs(stream, 1);
	}

	//stream << "}" << endl;
}