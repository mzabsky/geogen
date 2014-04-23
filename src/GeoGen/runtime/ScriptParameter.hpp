#pragma once

#include <string>

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
			std::string name;
			std::string label;
			std::string description;

		public:
			virtual ~ScriptParameter() {};
			ScriptParameter(std::string const& name, std::string const& label, std::string const& description) : name(name), label(label), description(description) { };

			inline std::string GetName() const { return this->name; }
			inline std::string GetLabel() const { return this->label; }
			inline std::string GetDescription() const { return this->description; }

			virtual ScriptParameterType GetType() const = 0;
			virtual void ResetToDefault() = 0;
			virtual bool EqualsTo(ScriptParameter const* other) const = 0;
			virtual ScriptParameter* Clone() const = 0;
		};
	}
}