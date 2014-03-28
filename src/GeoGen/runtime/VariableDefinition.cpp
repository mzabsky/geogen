#include "..\InternalErrorException.hpp"
#include "VariableDefinition.hpp"

using namespace geogen::runtime;

void VariableDefinition::Initialize(VirtualMachine& vm) const
{
	DynamicObject* defaultValue = this->CreateDefaultValue();

	if (defaultValue != NULL)
	{
		vm.GetMemoryManager().RegisterObject(defaultValue);
	}

	if (!vm.GetGlobalVariableTable().DeclareVariable(this->GetName(), defaultValue, this->IsConstant()))
	{
		throw InternalErrorException("Could not declare static object variable (already initialized or name conflict?).");
	}
}