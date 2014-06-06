#pragma once

#include "String.hpp"

namespace geogen
{
	enum Dimension
	{
		DIMENSION_WIDTH,
		DIMENSION_HEIGHT
	};

	String DimensionToString(Dimension dimensions);
}