#include "..\InternalErrorException.hpp"
#include "BooleanTypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "StaticObject.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;

DynamicObject* BooleanTypeDefinition::CreateInstance(VirtualMachine* vm, bool value) const
{
	return new BooleanObject(vm, this, value);
}

bool BooleanTypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceLessThan on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<BooleanObject const*>(a)->GetValue() < dynamic_cast<BooleanObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool BooleanTypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return dynamic_cast<BooleanObject const*>(a)->GetValue() == dynamic_cast<BooleanObject const*>(b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

DynamicObject* BooleanTypeDefinition::Copy(VirtualMachine* vm, DynamicObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using Copy on object of incorrect type.");
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return new BooleanObject(vm, this, dynamic_cast<BooleanObject const*>(a)->GetValue());
}