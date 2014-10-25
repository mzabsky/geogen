#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1405_ReservedScriptParameterNameUsed occurs.
		class ScriptParameterTypeNotSpecifiedException : public CompilerException
		{
		private:
			String parameterName;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param parameterName Name of the parameter.
			ScriptParameterTypeNotSpecifiedException(CodeLocation location, String const& parameterName) :
				CompilerException(GGE1405_ReservedScriptParameterNameUsed, location), parameterName(parameterName) {};

			/// Gets parameter name.
			/// @return The parameter name.
			inline String GetParameterName() const { return this->parameterName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Reserved script parameter \"" << parameterName << "\" was used on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " doesn't have its type specified and its type can't be inferred.";

				return ss.str();
			}
		};
	}
}