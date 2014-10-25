#include "BooleanScriptParameter.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

bool BooleanScriptParameter::EqualsTo(ScriptParameter const* other) const
{
	if (this->GetType() != other->GetType())
	{
		return false;
	}

	BooleanScriptParameter const* typedOther = dynamic_cast<BooleanScriptParameter const*>(other);

	return 
		this->GetName() == other->GetName() &&
		this->GetLabel() == other->GetLabel() && 
		this->GetDescription() == other->GetDescription() &&
		this->GetDefaultValue() == typedOther->GetDefaultValue();
}

BooleanScriptParameter::BooleanScriptParameter(BooleanScriptParameter const& other)
: BooleanScriptParameter(other.GetName(), other.GetLabel(), other.GetDescription(), other.GetDefaultValue())
{
	this->value = other.GetValue();
}

BooleanScriptParameter& BooleanScriptParameter::operator=(BooleanScriptParameter const& other)
{
	this->name = other.name;
	this->label = other.label;
	this->description = other.description;
	this->defaultValue = other.defaultValue;
	this->value = other.value;

	return *this;
}

ScriptParameter* BooleanScriptParameter::Clone() const
{
	return new BooleanScriptParameter(*this);
}

void BooleanScriptParameter::Serialize(IOStream& stream) const
{
	stream << GG_STR("Type: Boolean") << endl;
	stream << GG_STR("Name: ") << this->name << endl;
	stream << GG_STR("Label: ") << this->label << endl;
	stream << GG_STR("Description: ") << this->description << endl;
	stream << GG_STR("DefaultValue: ") << this->defaultValue << endl;
	stream << GG_STR("Value: ") << this->value << endl;
}