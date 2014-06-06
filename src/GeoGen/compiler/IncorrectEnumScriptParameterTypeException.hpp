#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectEnumScriptParameterTypeException : public CompilerException
		{
		private:
			String parameterName;
			String typeName;
		public:
			IncorrectEnumScriptParameterTypeException(CodeLocation location, String const& parameterName, String typeName) :
				CompilerException(GGE1413_IncorrectEnumScriptParameterType, location), parameterName(parameterName), typeName(typeName) {};

			inline String GetParameterName() const { return this->parameterName; }

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