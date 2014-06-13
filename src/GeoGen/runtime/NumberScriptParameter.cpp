#include "NumberScriptParameter.hpp"
#include "../InternalErrorException.hpp"
#include "../ApiUsageException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

NumberScriptParameter::NumberScriptParameter(String const& name, String const& label, String const& description, Number defaultValue, Number min, Number max, ScriptParameterValueRestriction restriction)
: ScriptParameter(name, label, description), value(defaultValue), defaultValue(defaultValue), min(min), max(max), restriction(restriction) 
{
	// Test min and max against that restriction.
	if (min != this->ApplyRestriction(min)){
		throw ApiUsageException(GG_STR("Min does not match the restriction"));
	}

	if (max != this->ApplyRestriction(max)){
		throw ApiUsageException(GG_STR("Max does not match the restriction"));
	}

	if (defaultValue != this->ApplyRestriction(defaultValue)){
		throw ApiUsageException(GG_STR("Default value does not match the restriction"));
	}
}

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

void NumberScriptParameter::SetValue(Number value) 
{
	if (value < this->min){
		value = this->min;
	}
	else if (value > this->max){
		value = this->max;
	}

	// TODO: Restriction can reduce the value below minimum.
	value = this->ApplyRestriction(value);

	this->value = value;
}

ScriptParameter* NumberScriptParameter::Clone() const
{
	return new NumberScriptParameter(*this);
}

Number NumberScriptParameter::ApplyRestriction(Number value) const
{
	if (this->restriction == SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED)
	{
		return value;
	}
	else if (this->restriction == SCRIPT_PARAMETER_VALUE_RESTRICTION_INTEGERS)
	{
		return floor(value);
	}
	else if (this->restriction == SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_2)
	{
		return 1U << (unsigned)(floor(log2(value > 0 ? value : 1)));
	}
	else if (this->restriction == SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_10)
	{
		return pow(10, floor(log10(value > 0 ? value : 1)));
	}
	else 
	{
		throw InternalErrorException(GG_STR("Invalid restriction."));
	}
}