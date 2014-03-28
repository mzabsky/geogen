#include "../InternalErrorException.hpp"
#include "TypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "StaticObject.hpp"

using namespace geogen::runtime;

void TypeDefinition::Initialize(VirtualMachine& vm)
{
	DynamicObject* staticInstance = this->CreateStaticObject();

	vm.GetMemoryManager().RegisterObject(staticInstance);

	if (!vm.GetGlobalVariableTable().DeclareVariable(this->GetName(), staticInstance, true))
	{
		throw InternalErrorException("Could not declare static object variable.");
	}
}

StaticObject* TypeDefinition::CreateStaticObject() const
{
	return new StaticObject(this);
}

bool TypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	return (!a->GetType());
}