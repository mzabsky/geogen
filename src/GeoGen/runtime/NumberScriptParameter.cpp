#include "NumberScriptParameter.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

bool NumberScriptParameter::EqualsTo(ScriptParameter const* other) const
{
	if (this->GetType() != other->GetType())
	{
		return false;
	}

	NumberScriptParameter const* typedOther = dynamic_cast<NumberScriptParameter const*>(other);

	return
		this->GetName() == other->GetName() &&
		this->GetLabel() == other->GetLabel() &&
		this->GetDescription() == other->GetDescription() &&
		this->GetDefaultValue() == typedOther->GetDefaultValue() &&
		this->GetMin() == typedOther->GetMin() &&
		this->GetMax() == typedOther->GetMax() &&
		this->GetRestriction() == typedOther->GetRestriction();
}

NumberScriptParameter::NumberScriptParameter(NumberScriptParameter const& other)
: NumberScriptParameter(other.GetName(), other.GetLabel(), other.GetDescription(), other.GetDefaultValue(), other.GetMin(), other.GetMax(), other.GetRestriction())
{
	this->value = other.GetValue();
}

NumberScriptParameter& NumberScriptParameter::operator=(NumberScriptParameter const& other)
{
	this->name = other.name;
	this->label = other.label;
	this->description = other.description;
	this->defaultValue = other.defaultValue;
	this->value = other.value;
	this->min = other.min;
	this->max = other.max;
	this->restriction = other.restriction;

	return *this;
}

ScriptParameter* NumberScriptParameter::Clone() const
{
	return new NumberScriptParameter(*this);
}