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