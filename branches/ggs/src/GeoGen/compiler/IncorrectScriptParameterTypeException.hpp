#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

/*namespace geogen
{
	namespace compiler
	{
		class IncorrectScriptParameterTypeException : public CompilerException
		{
		private:
			std::string parameterName;
			std::string parameterTypeName;
		public:
			IncorrectScriptParameterTypeException(CodeLocation location, std::string const& parameterName, std::string parameterTypeName) :
				CompilerException(GGE1405_IncorrectScriptParameterType, location), parameterName(parameterName), parameterTypeName(parameterTypeName) {};

			inline std::string GetParameterName() const { return this->parameterName; }

			inline std::string GetParameterTypeName() const { return this->parameterTypeName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Script parameter \"" << parameterName << "\" has incorrect type \"" << parameterTypeName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"Number\", \"Boolean\" or \"Enum\".";

				return ss.str();
			}
		};
	}
}*/