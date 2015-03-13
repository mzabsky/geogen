/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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

			virtual ~IncorrectScriptParameterValueRestrictionException() throw () {}

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