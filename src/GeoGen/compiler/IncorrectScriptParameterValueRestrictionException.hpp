#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectScriptParameterValueRestrictionException : public CompilerException
		{
		private:
			String parameterName;
			String restrictionName;
		public:
			IncorrectScriptParameterValueRestrictionException(CodeLocation location, String const& parameterName, String restrictionName) :
				CompilerException(GGE1407_IncorrectScriptParameterValueRestriction, location), parameterName(parameterName), restrictionName(restrictionName) {};

			inline String GetParameterName() const { return this->parameterName; }

			inline String GetRestrictionName() const { return this->restrictionName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Script parameter \"" << parameterName << "\" has incorrect value restriction \"" << restrictionName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}