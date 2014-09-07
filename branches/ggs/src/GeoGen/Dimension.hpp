#pragma once

#include "String.hpp"

namespace geogen
{
	/// Values that represent dimension.
	enum Dimension
	{
		/// Width. 
		DIMENSION_WIDTH,
		/// Height. 
		DIMENSION_HEIGHT
	};

	/// Converts Dimension to string.
	/// @param dimensions The dimension.
	/// @return The string.
	String DimensionToString(Dimension direction);
}