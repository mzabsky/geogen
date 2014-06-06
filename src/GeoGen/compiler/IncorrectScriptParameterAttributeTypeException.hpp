#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectScriptParameterAttributeTypeException : public CompilerException
		{
		private:
			String parameterName;
			String attributeName;
			String expectedAttributeTypeName;
			String actualAttributeTypeName;
		public:
			IncorrectScriptParameterAttributeTypeException(CodeLocation location, String const& parameterName, String attributeName, String expectedAttributeTypeName, String actualAttributeTypeName) :
				CompilerException(GGE1406_IncorrectScriptParameterAttributeType, location), parameterName(parameterName), attributeName(attributeName), expectedAttributeTypeName(expectedAttributeTypeName), actualAttributeTypeName(actualAttributeTypeName) {};

			inline String GetParameterName() const { return this->parameterName; }

			inline String GetAttributeName() const { return this->attributeName; }

			inline String GetExpectedAttributeTypeName() const { return this->expectedAttributeTypeName; }
			
			inline String GetActualAttributeTypeName() const { return this->actualAttributeTypeName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Script parameter \"" << parameterName << "\" has incorrect type of attribute \"" << attributeName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"" << expectedAttributeTypeName << "\", got \"" << actualAttributeTypeName << "\".";

				return ss.str();
			}
		};
	}
}