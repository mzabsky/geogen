#pragma once

#include <string>

#include "ScriptParameter.hpp"
#include "../Number.hpp"

namespace geogen
{
	namespace runtime
	{
		enum ScriptParameterValueRestriction
		{
			SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED,
			SCRIPT_PARAMETER_VALUE_RESTRICTION_INTEGERS,
			SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_2,
			SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_10,
		};

		class NumberScriptParameter : public ScriptParameter
		{
		private:
			Number defaultValue;
			Number value;
			Number min;
			Number max;
			ScriptParameterValueRestriction restriction;

			Number ApplyRestriction(Number value) const;
		public:
			NumberScriptParameter(String const& name, String const& label, String const& description, Number defaultValue, Number min, Number max, ScriptParameterValueRestriction restriction);
			NumberScriptParameter(NumberScriptParameter const&);
			NumberScriptParameter& operator=(NumberScriptParameter const&);

			virtual ScriptParameterType GetType() const { return SCRIPT_PARAMETER_TYPE_NUMBER; };

			inline Number GetDefaultValue() const { return this->defaultValue; }
			inline Number GetValue() const { return this->value; }
			inline Number GetMin() const { return this->min; }
			inline Number GetMax() const { return this->max; }
			inline ScriptParameterValueRestriction GetRestriction() const { return this->restriction; }

			void SetValue(Number value);

			virtual bool EqualsTo(ScriptParameter const* other) const;
			virtual void ResetToDefault() { this->value = this->defaultValue; };
			virtual ScriptParameter* Clone() const;
		};
	}
}