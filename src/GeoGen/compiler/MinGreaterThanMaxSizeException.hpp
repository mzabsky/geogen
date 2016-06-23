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
#include "../Direction.hpp"

namespace geogen
{
	namespace compiler
	{		
		/// Exception thrown when error geogen::GGE1412_MinGreaterThanMaxMapSize occurs.
		class MinGreaterThanMaxSizeException : public CompilerException
		{
		private:
			Direction direction;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param dimension The dimension.
			MinGreaterThanMaxSizeException(CodeLocation location, Direction dimension) :
				CompilerException(GGE1412_MinGreaterThanMaxMapSize, location), direction(direction) {};

			virtual ~MinGreaterThanMaxSizeException() throw () {}

			/// Gets the dimension.
			/// @return The dimension.
			inline Direction GetDimension() const { return this->direction; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "The minimum size was greater than the maximum size in dimension \"" << DirectionToString(direction) << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}