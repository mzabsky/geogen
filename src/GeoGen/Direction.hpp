#pragma once

#include "String.hpp"

namespace geogen
{
	/// Values that represent direction.
	enum Direction
	{
		/// Horizontal. 
		DIRECTION_HORIZONTAL,
		/// Vertical.
		DIRECTION_VERTICAL
	};

	/// Returns the other than the specified direction.
	/// @param d The direction.
	/// @return The other directions (vertical if horizontal, horizontal if vertical). Other values are returned unchanged.
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

	/// Converts Direction to string.
	/// @param direction The dimension.
	/// @return The string.
	String DirectionToString(Direction direction);
}