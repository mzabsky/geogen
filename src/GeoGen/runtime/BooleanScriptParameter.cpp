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