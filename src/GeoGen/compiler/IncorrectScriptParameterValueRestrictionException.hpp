#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1407_IncorrectScriptParameterValueRestriction occurs.
		class IncorrectScriptParameterValueRestrictionException : public CompilerException
		{
		private:
			String parameterName;
			String restrictionName;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param parameterName Name of the parameter.
			/// @param restrictionName Name of the restriction.
			IncorrectScriptParameterValueRestrictionException(CodeLocation location, String const& parameterName, String restrictionName) :
				CompilerException(GGE1407_IncorrectScriptParameterValueRestriction, location), parameterName(parameterName), restrictionName(restrictionName) {};

			/// Gets parameter name.
			/// @return The parameter name.
			inline String GetParameterName() const { return this->parameterName; }

			/// Gets restriction name.
			/// @return The restriction name.
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