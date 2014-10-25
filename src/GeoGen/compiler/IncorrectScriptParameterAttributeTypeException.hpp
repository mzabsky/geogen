#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1406_IncorrectScriptParameterAttributeType occurs.
		class IncorrectScriptParameterAttributeTypeException : public CompilerException
		{
		private:
			String parameterName;
			String attributeName;
			String expectedAttributeTypeName;
			String actualAttributeTypeName;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param parameterName Name of the parameter.
			/// @param attributeName Name of the attribute.
			/// @param expectedAttributeTypeName Name of the expected attribute type.
			/// @param actualAttributeTypeName Name of the actual attribute type.
			IncorrectScriptParameterAttributeTypeException(CodeLocation location, String const& parameterName, String attributeName, String expectedAttributeTypeName, String actualAttributeTypeName) :
				CompilerException(GGE1406_IncorrectScriptParameterAttributeType, location), parameterName(parameterName), attributeName(attributeName), expectedAttributeTypeName(expectedAttributeTypeName), actualAttributeTypeName(actualAttributeTypeName) {};

			/// Gets parameter name.
			/// @return The parameter name.
			inline String GetParameterName() const { return this->parameterName; }

			/// Gets attribute name.
			/// @return The attribute name.
			inline String GetAttributeName() const { return this->attributeName; }

			/// Gets expected attribute type name.
			/// @return The expected attribute type name.
			inline String GetExpectedAttributeTypeName() const { return this->expectedAttributeTypeName; }

			/// Gets actual attribute type name.
			/// @return The actual attribute type name.
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