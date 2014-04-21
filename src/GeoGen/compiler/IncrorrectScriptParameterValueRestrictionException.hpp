#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectScriptParameterValueRestrictionException : public CompilerException
		{
		private:
			std::string parameterName;
			std::string restrictionName;
		public:
			IncorrectScriptParameterValueRestrictionException(CodeLocation location, std::string const& parameterName, std::string restrictionName) :
				CompilerException(GGE1407_IncorrectScriptParameterValueRestriction, location), parameterName(parameterName), restrictionName(restrictionName) {};

			inline std::string GetParameterName() const { return this->parameterName; }

			inline std::string GetRestrictionName() const { return this->restrictionName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Script parameter \"" << parameterName << "\" has incorrect value restriction \"" << restrictionName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}