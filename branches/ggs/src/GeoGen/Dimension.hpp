#pragma once

#include <string>

namespace geogen
{
	enum Dimension
	{
		DIMENSION_WIDTH,
		DIMENSION_HEIGHT
	};

	std::string DimensionToString(Dimension dimensions);
}