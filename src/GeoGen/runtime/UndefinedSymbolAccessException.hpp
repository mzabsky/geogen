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

#include "RuntimeException.hpp"
#include "SymbolDefinitionException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2201_UndefinedFunction, geogen::GGE2202_UndefinedVariable and geogen::GGE2203_UndefinedMemberVariable occurs.
		class UndefinedSymbolAccessException : public SymbolDefinitionException
		{
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The code location.
			/// @param symbolName Name of the symbol.
			UndefinedSymbolAccessException(ErrorCode code, CodeLocation location, String const& symbolName) :
				SymbolDefinitionException(code, location, symbolName) {};

			virtual ~UndefinedSymbolAccessException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Undefined symbol \"" << this->GetSymbolName() << "\" accessed on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() <<  ".";
				return ss.str();
			}
		};
	}
}