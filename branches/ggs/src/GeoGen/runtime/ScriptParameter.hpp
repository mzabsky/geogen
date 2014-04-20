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
		private:
			std::string name;

			// Non-copyable
			ScriptParameter(ScriptParameter const&) {}
			ScriptParameter& operator=(ScriptParameter const&) {}
		public:
			virtual ~ScriptParameter() {};
			ScriptParameter(std::string const& name) : name(name) { };

			inline std::string GetName() const { return this->name; }

			virtual ScriptParameterType GetType() const = 0;
		};
	}
}