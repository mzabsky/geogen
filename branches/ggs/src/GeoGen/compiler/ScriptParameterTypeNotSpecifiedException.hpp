#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class ScriptParameterTypeNotSpecifiedException : public CompilerException
		{
		private:
			std::string parameterName;
		public:
			ScriptParameterTypeNotSpecifiedException(CodeLocation location, std::string const& parameterName) :
				CompilerException(GGE1404_ScriptParameterTypeNotSpecified, location), parameterName(parameterName) {};

			inline std::string GetParameterName() const { return this->parameterName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Script parameter \"" << parameterName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " doesn't have its type specified and its type can't be inferred.";

				return ss.str();
			}
		};
	}
}