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