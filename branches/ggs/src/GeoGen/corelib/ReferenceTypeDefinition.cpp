#include <memory>

#include "../InternalErrorException.hpp"
#include "ReferenceTypeDefinition.hpp"
#include "ScopeVariableReferenceObject.hpp"
#include "ArrayItemReferenceObject.hpp"
#include "ArrayObject.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;

ManagedObject* ReferenceTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	return a;
}

ManagedObject* ReferenceTypeDefinition::CreateScopeReferenceInstance(VirtualMachine* vm, VariableTableItem* variableTableItem) const
{
	auto_ptr<ManagedObject> object(new ScopeVariableReferenceObject(vm, this, variableTableItem));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

ManagedObject* ReferenceTypeDefinition::CreateArrayItemReferenceInstance(VirtualMachine* vm, ArrayObject* array, ManagedObject* key) const
{
	auto_ptr<ManagedObject> object(new ArrayItemReferenceObject(vm, this, array, key));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}