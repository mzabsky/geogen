#include "../InternalErrorException.hpp"
#include "TypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "StaticObject.hpp"

using namespace std;
using namespace geogen::runtime;

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
	return a < b;
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
		throw InternalErrorException("Invalid type conversion.");
	}
}