#include "EnumTypeDefinition.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

EnumTypeDefinition::EnumTypeDefinition(CodeLocation location, std::string const& name, ValueDefinitions valueDefinitions) : TypeDefinition(name), valueDefinitions(valueDefinitions)
{
	for (ValueDefinitions::const_iterator it = valueDefinitions.begin(); it != valueDefinitions.end(); it++){
		VariableDefinition* variableDefinition = new VariableDefinition(name, true);
		if (!this->GetVariableDefinitions().AddItem(variableDefinition, name)){
			delete variableDefinition;
			throw InternalErrorException("Bad list of enum values.");
		}
	}
}

DynamicObject* EnumTypeDefinition::CreateInstance(Number value) const
{
	return NULL;
}

bool EnumTypeDefinition::InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const
{
	return false;
}

bool EnumTypeDefinition::InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const
{
	return false;
}

DynamicObject* EnumTypeDefinition::Copy(DynamicObject* object) const
{
	return NULL;
}