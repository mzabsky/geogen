#include "../InternalErrorException.hpp"
#include "TypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "StaticObject.hpp"

using namespace geogen::runtime;

void TypeDefinition::Initialize(VirtualMachine* vm) const
{
	ManagedObject* staticInstance = this->CreateStaticObject(vm);

	vm->GetMemoryManager().RegisterObject(staticInstance);

	if (!vm->GetGlobalVariableTable().DeclareVariable(this->GetName(), staticInstance, true))
	{
		throw InternalErrorException("Could not declare static object variable (already initialized or name conflict?).");
	}
}

StaticObject* TypeDefinition::CreateStaticObject(VirtualMachine* vm) const
{
	return new StaticObject(vm, this);
}

bool TypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	return a < b;
}

bool TypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	return a == b;
}