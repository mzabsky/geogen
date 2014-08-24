#pragma once

#include "String.hpp"

namespace geogen
{
	enum Direction
	{
		DIRECTION_HORIZONTAL,
		DIRECTION_VERTICAL
	};

	inline Direction OtherDirection(Direction d)
	{
		switch (d)
		{
		case DIRECTION_HORIZONTAL:
			return DIRECTION_VERTICAL;
		case DIRECTION_VERTICAL:
			return DIRECTION_HORIZONTAL;
		default:
			return d;
		}
	}

	String DirectionToString(Direction dimensions);
}