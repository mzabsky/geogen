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

#include "CompilerException.hpp"
#include "../String.hpp"
#include "../Number.hpp"
#include "../runtime/ScriptParameters.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1411_IncorrectMapSizeNumericValue occurs.
		class IncorrectMapSizeNumericValueException : public CompilerException
		{
		private:
			Number actualValue;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param actualValue The actual value.
			IncorrectMapSizeNumericValueException(CodeLocation location, Number actualValue) :
				CompilerException(GGE1411_IncorrectMapSizeNumericValue, location), actualValue(actualValue) {};

			virtual ~IncorrectMapSizeNumericValueException() throw () {}

			/// Gets actual value.
			/// @return The actual value.
			inline Number GetActualValue() const { return this->actualValue; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Map size has to be a positive integer greater than or equal to " << runtime::MAP_SIZE_MIN << ", less than or equal to " << runtime::MAP_SIZE_MAX << ", was \"" << actualValue << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}