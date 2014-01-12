#include "..\InternalErrorException.hpp"
#include "..\Number.hpp"
#include "NumberObjectTypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "NumberObject.hpp"

using namespace geogen;
using namespace runtime;

DynamicObject* NumberObjectTypeDefinition::CreateInstance(Number value) const
{
	return new NumberObject(this, value);
}

bool NumberObjectTypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using InstanceLessThan on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return ((NumberObject const*)a)->GetValue() < ((NumberObject const*)b)->GetValue();
	}

	return TypeDefinition::InstanceLessThan(a, b);
}

bool NumberObjectTypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using InstanceEqualsTo on object of incorrect type.");
	}

	if (b->GetType() == this)
	{
		return ((NumberObject const*)a)->GetValue() == ((NumberObject const*)b)->GetValue();
	}

	return TypeDefinition::InstanceEqualsTo(a, b);
}

DynamicObject* NumberObjectTypeDefinition::Copy(DynamicObject* a) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using Copy on object of incorrect type.");
	}

	return new NumberObject(this, ((NumberObject const*)a)->GetValue());
}