#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"
#include "..\Dimension.hpp"

namespace geogen
{
	namespace compiler
	{
		class MinGreaterThanMaxSizeException : public CompilerException
		{
		private:
			Dimension dimension;
		public:
			MinGreaterThanMaxSizeException(CodeLocation location, Dimension dimension) :
				CompilerException(GGE1412_MinGreaterThanMaxMapSize, location), dimension(dimension) {};

			inline Dimension GetDimension() const { return this->dimension; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "The minimum size was greater than the maximum size in dimension \"" << DimensionToString(dimension) << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}