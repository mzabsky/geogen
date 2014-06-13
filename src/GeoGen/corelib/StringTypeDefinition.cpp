#include "StringTypeDefinition.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"

using namespace geogen;
using namespace runtime;
using namespace corelib;
using namespace std;

ManagedObject* StringTypeDefinition::CreateInstance(VirtualMachine* vm, String const& value) const
{
	auto_ptr<ManagedObject> object(new StringObject(vm, this, value));
	vm->GetMemoryManager().RegisterObject(object.get());
	return object.release();
}

bool StringTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceLessThan on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<StringObject const*>(a)->GetValue() < dynamic_cast<StringObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool StringTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using InstanceEqualsTo on object of incorrect type."));
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<StringObject const*>(a)->GetValue() == dynamic_cast<StringObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* StringTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException(GG_STR("Using Copy on object of incorrect type."));
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	// Value type, copies
	return CreateInstance(vm, static_cast<StringObject const*>(a)->GetValue());
}