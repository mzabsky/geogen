#pragma once

#include "String.hpp"

namespace geogen
{
	enum Direction
	{
		DIRECTION_HORIZONTAL,
		DIRECTION_VERTICAL
	};

	String DirectionToString(Direction dimensions);
}