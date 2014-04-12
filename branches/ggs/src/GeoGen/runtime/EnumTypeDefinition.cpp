#include "../InternalErrorException.hpp"
#include "EnumTypeDefinition.hpp"
#include "NumberTypeDefinition.hpp"
#include "VariableDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

EnumTypeDefinition::EnumTypeDefinition(CodeLocation location, std::string const& name, ValueDefinitions valueDefinitions) : TypeDefinition(name), valueDefinitions(valueDefinitions)
{
	for (ValueDefinitions::const_iterator it = valueDefinitions.begin(); it != valueDefinitions.end(); it++){
		VariableDefinition* variableDefinition = new VariableDefinition(it->second, true);
		if (!this->GetStaticVariableDefinitions().AddItem(variableDefinition)){
			delete variableDefinition;
			throw InternalErrorException("Bad list of enum values.");
		}
	}
}

DynamicObject* EnumTypeDefinition::CreateInstance(Number value) const
{
	// TODO: naplnit membery ZDE!
	return new NumberObject(this, value);
}

bool EnumTypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
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

bool EnumTypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
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

DynamicObject* EnumTypeDefinition::Copy(DynamicObject* a) const
{
	if (a->GetType() != this)
	{
		throw InternalErrorException("Using Copy on object of incorrect type.");
	}

	if (a->IsStaticObject())
	{
		return a;
	}

	return new NumberObject(this, ((NumberObject const*)a)->GetValue());
}