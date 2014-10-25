#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1413_IncorrectEnumScriptParameterType occurs.
		class IncorrectEnumScriptParameterTypeException : public CompilerException
		{
		private:
			String parameterName;
			String typeName;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param parameterName Name of the parameter.
			/// @param typeName Name of the type.
			IncorrectEnumScriptParameterTypeException(CodeLocation location, String const& parameterName, String typeName) :
				CompilerException(GGE1413_IncorrectEnumScriptParameterType, location), parameterName(parameterName), typeName(typeName) {};

			/// Gets parameter name.
			/// @return The parameter name.
			inline String GetParameterName() const { return this->parameterName; }

			/// Gets type name.
			/// @return The type name.
			inline String GetTypeName() const { return this->typeName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Enum script parameter \"" << parameterName << "\" references type \"" << typeName << "\" which is not a proper enum type on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}