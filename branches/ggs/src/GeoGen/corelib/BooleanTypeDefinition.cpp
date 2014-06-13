#include "BooleanTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

ManagedObject* BooleanTypeDefinition::CreateInstance(VirtualMachine* vm, bool value) const
{
	auto_ptr<ManagedObject> object(new BooleanObject(vm, this, value));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool BooleanTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<BooleanObject const*>(a)->GetValue() < dynamic_cast<BooleanObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool BooleanTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<BooleanObject const*>(a)->GetValue() == dynamic_cast<BooleanObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* BooleanTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return CreateInstance(vm, dynamic_cast<BooleanObject const*>(a)->GetValue());
}