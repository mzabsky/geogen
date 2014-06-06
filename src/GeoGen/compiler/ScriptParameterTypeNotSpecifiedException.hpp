#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class ScriptParameterTypeNotSpecifiedException : public CompilerException
		{
		private:
			String parameterName;
		public:
			ScriptParameterTypeNotSpecifiedException(CodeLocation location, String const& parameterName) :
				CompilerException(GGE1404_ScriptParameterTypeNotSpecified, location), parameterName(parameterName) {};

			inline String GetParameterName() const { return this->parameterName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Script parameter \"" << parameterName << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " doesn't have its type specified and its type can't be inferred.";

				return ss.str();
			}
		};
	}
}