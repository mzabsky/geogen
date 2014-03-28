#include "..\InternalErrorException.hpp"
#include "..\Number.hpp"
#include "NumberTypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "StaticObject.hpp"

using namespace geogen;
using namespace runtime;
using namespace std;

DynamicObject* NumberTypeDefinition::CreateInstance(Number value) const
{
	return new NumberObject(this, value);
}

bool NumberTypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
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

bool NumberTypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
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

DynamicObject* NumberTypeDefinition::Copy(DynamicObject* a) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using Copy on object of incorrect type.");
	}

	return new NumberObject(this, ((NumberObject const*)a)->GetValue());
}