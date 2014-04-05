#include "..\InternalErrorException.hpp"
#include "BooleanTypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "StaticObject.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;

DynamicObject* BooleanTypeDefinition::CreateInstance(bool value) const
{
	return new BooleanObject(this, value);
}

bool BooleanTypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using InstanceLessThan on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return ((BooleanObject const*)a)->GetValue() < ((BooleanObject const*)b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool BooleanTypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return ((BooleanObject const*)a)->GetValue() == ((BooleanObject const*)b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

DynamicObject* BooleanTypeDefinition::Copy(DynamicObject* a) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using Copy on object of incorrect type.");
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return new BooleanObject(this, ((BooleanObject const*)a)->GetValue());
}