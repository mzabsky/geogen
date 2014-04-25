#include "..\InternalErrorException.hpp"
#include "..\Number.hpp"
#include "NumberTypeDefinition.hpp"
#include "ManagedObject.hpp"
#include "StaticObject.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;

ManagedObject* NumberTypeDefinition::CreateInstance(VirtualMachine* vm, Number value) const
{
	return new NumberObject(vm, this, value);
}

bool NumberTypeDefinition::InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceLessThan on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return ((NumberObject const*)a)->GetValue() < ((NumberObject const*)b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool NumberTypeDefinition::InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return ((NumberObject const*)a)->GetValue() == ((NumberObject const*)b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

ManagedObject* NumberTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using Copy on object of incorrect type.");
	}

	if (a->IsStaticObject())
	{
		return a;
	}

    // Value type, copies
	return new NumberObject(vm, this, ((NumberObject const*)a)->GetValue());
}