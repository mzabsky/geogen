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
		/// Exception thrown when error geogen::GGE1410_UndefinedMetadataIdentifier occurs.
		class UndefinedMetadataIdentifierException : public CompilerException
		{
		private:
			String identifier;
		public:
			UndefinedMetadataIdentifierException(CodeLocation location, String const& identifier) :
				CompilerException(GGE1410_UndefinedMetadataIdentifier, location), identifier(identifier) {};

			virtual ~UndefinedMetadataIdentifierException() throw () {}

			inline String GetIdentifier() const { return this->identifier; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Undefined identifier \"" << identifier << "\" refernced in script metadata on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"Number\", \"Boolean\" or \"Enum\".";

				return ss.str();
			}
		};
	}
}