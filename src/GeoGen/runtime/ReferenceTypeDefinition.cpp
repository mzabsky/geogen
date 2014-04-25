#include "..\InternalErrorException.hpp"
#include "ReferenceTypeDefinition.hpp"
#include "ManagedObject.hpp"

using namespace geogen;
using namespace runtime;

ManagedObject* ReferenceTypeDefinition::Copy(ManagedObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using Copy on object of incorrect type.");
	}

	return a;
}