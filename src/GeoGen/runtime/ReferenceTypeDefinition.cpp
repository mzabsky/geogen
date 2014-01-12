#include "..\InternalErrorException.hpp"
#include "ReferenceTypeDefinition.hpp"
#include "DynamicObject.hpp"

using namespace geogen;
using namespace runtime;

DynamicObject* ReferenceTypeDefinition::Copy(DynamicObject* a) const
{
	if (a->GetType() != this)
	{
		throw new InternalErrorException("Using Copy on object of incorrect type.");
	}

	return a;
}