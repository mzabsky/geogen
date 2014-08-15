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
	stream << "Name: " << this->name << endl;
	stream << "Label: " << this->label << endl;
	stream << "Description: " << this->description << endl;
	stream << "DefaultValue: " << this->defaultValue << endl;
	stream << "Value: " << this->value << endl;
}