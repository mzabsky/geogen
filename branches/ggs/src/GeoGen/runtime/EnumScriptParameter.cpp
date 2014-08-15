#include "EnumScriptParameter.hpp"
#include "../InternalErrorException.hpp"
#include "../ApiUsageException.hpp"
#include "ManagedObject.hpp"
#include "../corelib/EnumTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::corelib;

EnumScriptParameter::EnumScriptParameter(String const& name, String const& label, String const& description, EnumTypeDefinition const* enumType, int defaultValue)
: ScriptParameter(name, label, description), enumType(enumType)
{
	if (this->GetEnumType()->IsValueIntDefined(defaultValue))
	{
		this->defaultValue = this->value = defaultValue;
	}
	else {
		this->defaultValue = this->value = this->GetEnumType()->GetDefaultValueInt();
	}
}

bool EnumScriptParameter::EqualsTo(ScriptParameter const* other) const
{
	if (this->GetType() != other->GetType())
	{
		return false;
	}

	EnumScriptParameter const* typedOther = dynamic_cast<EnumScriptParameter const*>(other);

	return
		this->GetName() == other->GetName() &&
		this->GetLabel() == other->GetLabel() &&
		this->GetDescription() == other->GetDescription() &&
		this->GetDefaultValue() == typedOther->GetDefaultValue() &&
		this->GetEnumType() == typedOther->GetEnumType();
}

EnumScriptParameter::EnumScriptParameter(EnumScriptParameter const& other)
: EnumScriptParameter(other.GetName(), other.GetLabel(), other.GetDescription(), other.GetEnumType(), other.GetDefaultValue())
{
	this->value = other.GetValue();
}

EnumScriptParameter& EnumScriptParameter::operator=(EnumScriptParameter const& other)
{
	this->name = other.name;
	this->label = other.label;
	this->description = other.description;
	this->defaultValue = other.defaultValue;
	this->value = other.value;
	this->enumType = other.enumType;

	return *this;
}

void EnumScriptParameter::SetValue(int value)
{
	if (this->GetEnumType()->IsValueIntDefined(value))
	{
		this->value = value;
	}
	else {
		this->value = this->GetEnumType()->GetDefaultValueInt();
	}
}

ScriptParameter* EnumScriptParameter::Clone() const
{
	return new EnumScriptParameter(*this);
}

void EnumScriptParameter::Serialize(IOStream& stream) const
{
	stream << "Name: " << this->name << endl;
	stream << "Label: " << this->label << endl;
	stream << "Description: " << this->description << endl;
	stream << "DefaultValue: " << this->defaultValue << endl;
	stream << "Value: " << this->value << endl;
	stream << "EnumType: " << this->enumType << endl;
}