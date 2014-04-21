#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectScriptParameterAttributeTypeException : public CompilerException
		{
		private:
			std::string parameterName;
			std::string attributeName;
			std::string expectedAttributeTypeName;
			std::string actualAttributeTypeName;
		public:
			IncorrectScriptParameterAttributeTypeException(CodeLocation location, std::string const& parameterName, std::string attributeName, std::string expectedAttributeTypeName, std::string actualAttributeTypeName) :
				CompilerException(GGE1406_IncorrectScriptParameterAttributeType, location), parameterName(parameterName), attributeName(attributeName), expectedAttributeTypeName(expectedAttributeTypeName), actualAttributeTypeName(actualAttributeTypeName) {};

			inline std::string GetParameterName() const { return this->parameterName; }

			inline std::string GetAttributeName() const { return this->attributeName; }

			inline std::string GetExpectedAttributeTypeName() const { return this->expectedAttributeTypeName; }
			
			inline std::string GetActualAttributeTypeName() const { return this->actualAttributeTypeName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Script parameter \"" << parameterName << "\" has incorrect type of attribute \"" << attributeName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"" << expectedAttributeTypeName << "\", got \"" << actualAttributeTypeName << "\".";

				return ss.str();
			}
		};
	}
}