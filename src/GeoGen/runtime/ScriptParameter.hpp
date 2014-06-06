#pragma once

#include "../String.hpp"

namespace geogen
{
	namespace runtime
	{
		enum ScriptParameterType
		{
			SCRIPT_PARAMETER_TYPE_NUMBER,
			SCRIPT_PARAMETER_TYPE_BOOLEAN,
			SCRIPT_PARAMETER_TYPE_ENUM
		};

		class ScriptParameter
		{
		protected:
			String name;
			String label;
			String description;

		public:
			virtual ~ScriptParameter() {};
			ScriptParameter(String const& name, String const& label, String const& description) : name(name), label(label), description(description) { };

			inline String GetName() const { return this->name; }
			inline String GetLabel() const { return this->label; }
			inline String GetDescription() const { return this->description; }

			virtual ScriptParameterType GetType() const = 0;
			virtual void ResetToDefault() = 0;
			virtual bool EqualsTo(ScriptParameter const* other) const = 0;
			virtual ScriptParameter* Clone() const = 0;
		};
	}
}