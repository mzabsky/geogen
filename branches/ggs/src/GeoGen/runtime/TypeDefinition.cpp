#include "TypeDefinition.hpp"
#include "DynamicObject.hpp"
#include "StaticObject.hpp"

using namespace geogen::runtime;

StaticObject* TypeDefinition::CreateStaticObject() const
{
	return new StaticObject(this);
}

bool TypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	return (!a->GetType());
}