#pragma once

#include <string>

#include "ScriptParameter.hpp"

namespace geogen
{
	namespace runtime
	{
		class BooleanScriptParameter : public ScriptParameter
		{
		private:
			bool defaultValue;
			bool value;
		public:
			BooleanScriptParameter(std::string const& name, std::string const& label, std::string const& description, bool defaultValue) : ScriptParameter(name, label, description), value(defaultValue), defaultValue(defaultValue) {};

			virtual ScriptParameterType GetType() const { return SCRIPT_PARAMETER_TYPE_BOOLEAN; };

			inline bool GetDefaultValue() const { return this->defaultValue; }
			inline bool GetValue() const { return this->value; }
			inline void SetValue(bool value) { this->value = value; }

			virtual void ResetToDefault() { this->value = this->defaultValue; };
			virtual bool EqualsTo(ScriptParameter const* other) const;
		};
	}
}