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
		/// Exception thrown when errors geogen::GGE1402_ScriptParametersNotKeyValueCollection, geogen::GGE1403_ScriptParameterNotKeyValueCollection, geogen::GGE1408_WidthNotKeyValueCollection and geogen::GGE1409_HeightNotKeyValueCollection occur.
		class IncorrectMetadataValueTypeException : public CompilerException
		{
		private:
			String valueName;
			String expectedType;
			String actualType;
		public:

			/// Constructor.
			/// @param errorCode The error code.
			/// @param location The location.
			/// @param valueName The value name.
			/// @param expectedType The expected type.
			/// @param actualType The actual type.
			IncorrectMetadataValueTypeException(ErrorCode errorCode, CodeLocation location, String const& valueName, String const& expectedType, String const& actualType) :
				CompilerException(errorCode, location), valueName(valueName), expectedType(expectedType), actualType(actualType) {};

			virtual ~IncorrectMetadataValueTypeException() throw () {}

			/// Gets value name.
			/// @return The value name.
			inline String GetValueName() const { return this->valueName; }

			/// Gets expected type.
			/// @return The expected type.
			inline String GetExpectedType() const { return this->expectedType; }

			/// Gets actual type.
			/// @return The actual type.
			inline String GetActualType() const { return this->actualType; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Metadata value \"" << valueName << "\" expected to be of type \"" << expectedType << "\", had \"" << actualType << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}