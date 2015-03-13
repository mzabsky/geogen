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
		/// Exception thrown when errors geogen::GGE1310_EnumValueNotInteger and geogen::GGE1311_ReservedTypeNameUsed occur.
		class InvalidSymbolDefinitionException : public CompilerException
		{
		private:
			String symbolName;
		public:

			/// Constructor.
			/// @param code The code.
			/// @param location The location.
			/// @param symbolName Name of the symbol.
			explicit InvalidSymbolDefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			virtual ~InvalidSymbolDefinitionException() throw () {}

			/// Gets symbol name.
			/// @return The symbol name.
			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Symbol \"" << symbolName << "\" has bad definition.";
				return ss.str();
			}
		};
	}
}