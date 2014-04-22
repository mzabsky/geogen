#include "../InternalErrorException.hpp"
#include "TypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "StaticObject.hpp"

using namespace geogen::runtime;

void TypeDefinition::Initialize(VirtualMachine* vm) const
{
	DynamicObject* staticInstance = this->CreateStaticObject(vm);

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

bool TypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
{
	return a < b;
}

bool TypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	return a == b;
}