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

#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1201_UnexpectedToken occurs.
		class UnexpectedTokenException : public CompilerException
		{
		private:
			String expectedTokenName;
			/*String actualTokenName;
			String actualTokenText;*/
		public:

			/// Constructor.
			/// @param code The code.
			/// @param location The location.
			/// @param expectedTokenName Name of the expected token.
			explicit UnexpectedTokenException(ErrorCode code, CodeLocation location, String const& expectedTokenName/*, String const& actualTokenName, String const& actualTokenText*/) :
				CompilerException(code, location), expectedTokenName(expectedTokenName)/*, actualTokenName(actualTokenName), actualTokenText(actualTokenText)*/
			{				
			};

			virtual ~UnexpectedTokenException() throw () {}

			/// Gets expected token name.
			/// @return The expected token name.
			inline String GetExpectedTokenName()
			{
				return expectedTokenName;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Unexpected token on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ", expected \"" << expectedTokenName << "\".";
				return ss.str();
			}
		};
	}
}