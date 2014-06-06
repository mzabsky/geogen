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
			String parameterName;
			String parameterTypeName;
		public:
			IncorrectScriptParameterTypeException(CodeLocation location, String const& parameterName, String parameterTypeName) :
				CompilerException(GGE1405_IncorrectScriptParameterType, location), parameterName(parameterName), parameterTypeName(parameterTypeName) {};

			inline String GetParameterName() const { return this->parameterName; }

			inline String GetParameterTypeName() const { return this->parameterTypeName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Script parameter \"" << parameterName << "\" has incorrect type \"" << parameterTypeName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"Number\", \"Boolean\" or \"Enum\".";

				return ss.str();
			}
		};
	}
}*/