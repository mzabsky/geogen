#include "TypeDefinition.hpp"
#include "DynamicObject.hpp"

using namespace geogen::runtime;

bool TypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	return (!a->GetType());
}