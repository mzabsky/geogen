#pragma once

#include <stdexcept>

#include "CompilerException.hpp"
#include "..\Direction.hpp"

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