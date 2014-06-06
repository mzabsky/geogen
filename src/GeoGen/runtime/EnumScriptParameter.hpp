#pragma once

#include <string>

#include "ScriptParameter.hpp"
#include "../Number.hpp"

namespace geogen
{
	namespace corelib 
	{
		class EnumTypeDefinition;
	}

	namespace runtime
	{
		class EnumScriptParameter : public ScriptParameter
		{
		private:
			int value;
			int defaultValue;
			corelib::EnumTypeDefinition const* enumType;
		public:
			EnumScriptParameter(std::string const& name, std::string const& label, std::string const& description, corelib::EnumTypeDefinition const* enumType, int defaultValue);
			EnumScriptParameter(EnumScriptParameter const&);
			EnumScriptParameter& operator=(EnumScriptParameter const&);

			virtual ScriptParameterType GetType() const { return SCRIPT_PARAMETER_TYPE_ENUM; };

			inline corelib::EnumTypeDefinition const* GetEnumType() const { return this->enumType; }

			inline int GetDefaultValue() const { return this->defaultValue; }
			inline int GetValue() const { return this->value; }
			
			void SetValue(int value);

			virtual bool EqualsTo(ScriptParameter const* other) const;
			virtual void ResetToDefault() { this->value = this->defaultValue; };
			virtual ScriptParameter* Clone() const;
		};
	}
}